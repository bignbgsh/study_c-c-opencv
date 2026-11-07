#include "armor_detector.h"

#include <opencv2/geometry.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace vision{

std::string colorName(TargetColor color) {
    return color == TargetColor::Red ? "Red" : "Blue";
}

namespace{

cv::Point toPixel(const cv::Point2f& p){
    return cv::Point(
        cvRound(p.x),
        cvRound(p.y)
    );
}

void drawTarget(cv::Mat& image,
                const cv::RotatedRect& box,
                const cv::Scalar& color){
                    cv::Point2f corners[4];
                    box.points(corners);
                    for (int i = 0; i < 4; ++i) {
                        cv::line(image, toPixel(corners[i]), toPixel(corners[(i + 1) % 4]), color, 2);
                    }
                }

cv::Mat cleanMask(const cv::Mat& mask){

    const cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    const cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));

    cv::Mat closed;
    cv::Mat opened;

    cv::morphologyEx(
        mask,
        closed,
        cv::MORPH_CLOSE,
        kernel3
    );

    cv::morphologyEx(
        closed,
        opened,
        cv::MORPH_OPEN,
        kernel2

    );
    return opened;


}

void extractLights(
    const cv::Mat& mask,
    TargetColor color,
    std::vector<LightBar>& lights,
    const DetectionParams& params
){

    cv::Mat contourInput = mask.clone();

    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(
        contourInput,
        contours,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE
    );

    for (const auto& contour : contours) {

        //面积
        const double area = cv::contourArea(contour);

        if(area < params.minContourArea || area > params.maxContourArea){
            continue;
        }

        //最小旋转矩形
        const cv::RotatedRect box = cv::minAreaRect(contour);

        const float length = std::max(
            box.size.width,
            box.size.height
        );
        
        const float width = std::min(
            box.size.width,
            box.size.height
        );

        //灯条尺寸
        if(length < params.minLightLength || width < params.minLightWidth){
            continue;
        }

        //长宽比
        const float aspect = length / width;
        if(aspect < params.minLightAspect || aspect > params.maxLightAspect){
            continue;
        }

        //获取四个顶点
        cv::Point2f pts[4];
        box.points(pts);

        //计算向量
        const cv::Point2f e01 = pts[1] - pts[0];
        const cv::Point2f e12 = pts[2] - pts[1];

        const float len01 = std::hypot(e01.x, e01.y);

        const float len12 = std::hypot(e12.x, e12.y);

        cv::Point2f axis = len01 > len12 ? e01 : e12;

        const float axisLength = std::hypot(axis.x, axis.y);

        if(axisLength <= 1e-6f){
            continue;
        }

        axis.x /= axisLength;
        axis.y /= axisLength;

        if (axis.y < 0) {
            axis.x = -axis.x;
            axis.y = -axis.y;
        }

        //计算倾斜角
        const double tiltDeg = std::atan2(
            static_cast<double>(axis.x),
            static_cast<double>(axis.y)
        ) * 180.0 / CV_PI;

        if(std::abs(tiltDeg) > params.maxLightTiltDeg){
            continue;
        }

        //保存灯条信息
        LightBar light;

        light.color = color;
        light.area = area;
        light.box = box;
        light.center = box.center;
        light.length = length;
        light.width = width;
        light.tiltDeg = tiltDeg;

        const float halfLength = length * 0.5f;

        // 长轴上端点
        light.top = cv::Point2f(
            box.center.x - axis.x * halfLength,
            box.center.y - axis.y * halfLength
        );

        // 长轴下端点
        light.bottom = cv::Point2f(
            box.center.x + axis.x * halfLength,
            box.center.y + axis.y * halfLength
        );

        light.id = static_cast<int>(lights.size());

        lights.push_back(light);
    }
}

//------判断灯条能否配对

bool tryPair(
    const LightBar& first,
    const LightBar& second,
    const DetectionParams& params,
    ArmorCandidate& armor
){
    //  颜色必须相同
    if (first.color != second.color) {
        return false;
    }

    // 2. 确定左右顺序
    const LightBar* left = &first;
    const LightBar* right = &second;

    if (left->center.x > right->center.x) {
        std::swap(left, right);
    }


    // 3. 平均长度和长度比例
    const double minLength = std::min(
        left->length,
        right->length
    );

    if (minLength <= 0.0) {
        return false;
    }

    const double maxLength = std::max(
        left->length,
        right->length
    );

    const double lengthRatio =
        maxLength / minLength;

    if (lengthRatio > params.maxLengthRatio) {
        return false;
    }

    const double avgLength =
        (left->length + right->length) / 2.0;

     // 4. 灯条中心距离
    const double dx =
        right->center.x - left->center.x;

    const double dy = std::abs(
        right->center.y - left->center.y
    );

    const double spacingRatio = dx / avgLength;
    const double yOffsetRatio = dy / avgLength;

    if (spacingRatio < params.minSpacingRatio ||
        spacingRatio > params.maxSpacingRatio) {
        return false;
    }

    if (yOffsetRatio > params.maxYOffsetRatio) {
        return false;
    }

    // 5. 倾斜角差
    const double angleDiff = std::abs(
        left->tiltDeg - right->tiltDeg
    );

    if (angleDiff > params.maxAngleDiffDeg) {
        return false;
    }

    // 6. 生成装甲板候选
    armor.color = left->color;
    armor.leftLightId = left->id;
    armor.rightLightId = right->id;

    // 固定角点顺序：
    // 左上、右上、右下、左下
    armor.corners = {
        left->top,
        right->top,
        right->bottom,
        left->bottom
    };

    // 7. 计算装甲板中心
    cv::Point2f sum(0.0f, 0.0f);

    for (const auto& p : armor.corners) {
        sum += p;
    }

    armor.center = sum * 0.25f;

    // 8. 计算装甲板包围矩形
    std::vector<cv::Point2f> cornerPoints(
        armor.corners.begin(),
        armor.corners.end()
    );

    armor.boundingBox =
        cv::boundingRect(cornerPoints);

    // 9. 计算配对评分：越小越好
    armor.score =
        2.0 * (lengthRatio - 1.0)
        + 2.0 * yOffsetRatio
        + angleDiff / 15.0
        + 0.2 * std::abs(spacingRatio - 2.5);

    armor.valid = true;

    return true;






    }

// ---------- 辅助函数6：避免重复使用灯条 ----------
std::vector<ArmorCandidate> pairLights(
    const std::vector<LightBar>& lights,
    const DetectionParams& params
) {
    std::vector<ArmorCandidate> candidates;

    // 枚举所有不重复的两两组合
    for (std::size_t i = 0; i < lights.size(); ++i) {
        for (std::size_t j = i + 1; j < lights.size(); ++j) {

            ArmorCandidate armor;

            if (tryPair(
                    lights[i],
                    lights[j],
                    params,
                    armor
                )) {
                candidates.push_back(armor);
            }
        }
    }

    // 按评分从低到高排序
    std::sort(
        candidates.begin(),
        candidates.end(),
        [](const ArmorCandidate& a,
           const ArmorCandidate& b) {
            return a.score < b.score;
        }
    );

    std::vector<ArmorCandidate> selected;

    // 标记已经用于最终配对的灯条
    std::vector<bool> used(lights.size(), false);

    for (const auto& armor : candidates) {

        const int leftId = armor.leftLightId;
        const int rightId = armor.rightLightId;

        if (used[leftId] || used[rightId]) {
            continue;
        }

        used[leftId] = true;
        used[rightId] = true;

        ArmorCandidate accepted = armor;

        accepted.id =
            static_cast<int>(selected.size());

        selected.push_back(accepted);
    }

    return selected;
}
}

// ========== 主检测函数 ==========
DetectionResult detectArmors(
    const cv::Mat& image,
    const DetectionParams& params
) {
    // 1. 检查输入图片
    if (image.empty() || image.channels() != 3) {
        throw std::invalid_argument(
            "Input must be a non-empty 3-channel BGR image"
        );
    }

    DetectionResult result;

    // 2. 转换为 HSV
    cv::Mat hsv;

    cv::cvtColor(
        image,
        hsv,
        cv::COLOR_BGR2HSV
    );

    // 3. 提取两段红色范围
    cv::Mat redPart1;
    cv::Mat redPart2;

    cv::inRange(
        hsv,
        params.redLow1,
        params.redHigh1,
        redPart1
    );

    cv::inRange(
        hsv,
        params.redLow2,
        params.redHigh2,
        redPart2
    );

    cv::bitwise_or(
        redPart1,
        redPart2,
        result.redMaskRaw
    );

    // 4. 蓝色分割
    cv::Mat blueRaw;

    cv::inRange(
        hsv,
        params.blueLow,
        params.blueHigh,
        blueRaw
    );

    // 5. 形态学清理
    result.redMask = cleanMask(result.redMaskRaw);
    result.blueMask = cleanMask(blueRaw);

    // 6. 提取红色灯条
    extractLights(
        result.redMask,
        TargetColor::Red,
        result.lights,
        params
    );

    // 7. 提取蓝色灯条
    extractLights(
        result.blueMask,
        TargetColor::Blue,
        result.lights,
        params
    );

    // 8. 灯条配对
    result.armors = pairLights(
        result.lights,
        params
    );

    // ========== 绘制灯条候选图 ==========
    result.lightDebug = image.clone();

    for (const auto& light : result.lights) {

        const cv::Scalar color =
            light.color == TargetColor::Red
            ? cv::Scalar(0, 0, 255)
            : cv::Scalar(255, 0, 0);

        drawTarget(
            result.lightDebug,
            light.box,
            color
        );

        cv::circle(
            result.lightDebug,
            toPixel(light.center),
            3,
            cv::Scalar(0, 255, 255),
            cv::FILLED
        );

        cv::putText(
            result.lightDebug,
            "L" + std::to_string(light.id),
            toPixel(light.top) + cv::Point(5, -5),
            cv::FONT_HERSHEY_SIMPLEX,
            0.5,
            cv::Scalar(255, 255, 255),
            1
        );
    }

    // ========== 绘制装甲板结果图 ==========
    result.armorDebug = image.clone();

    for (const auto& armor : result.armors) {

        // 1. 普通外接框
        cv::rectangle(
            result.armorDebug,
            armor.boundingBox,
            cv::Scalar(255, 255, 0),
            2
        );

        // 2. 装甲板四边形及角点编号
        for (int i = 0; i < 4; ++i) {

            const cv::Point p1 =
                toPixel(armor.corners[i]);

            const cv::Point p2 =
                toPixel(armor.corners[(i + 1) % 4]);

            cv::line(
                result.armorDebug,
                p1,
                p2,
                cv::Scalar(0, 255, 0),
                2
            );

            cv::circle(
                result.armorDebug,
                p1,
                4,
                cv::Scalar(0, 0, 255),
                cv::FILLED
            );

            cv::putText(
                result.armorDebug,
                std::to_string(i),
                p1 + cv::Point(5, -5),
                cv::FONT_HERSHEY_SIMPLEX,
                0.5,
                cv::Scalar(255, 255, 255),
                1
            );
        }

        // 3. 装甲板中心点
        cv::circle(
            result.armorDebug,
            toPixel(armor.center),
            5,
            cv::Scalar(0, 255, 255),
            cv::FILLED
        );

        // 4. 装甲板编号
        cv::putText(
            result.armorDebug,
            "Armor " + std::to_string(armor.id),
            cv::Point(
                armor.boundingBox.x,
                std::max(15, armor.boundingBox.y - 10)
            ),
            cv::FONT_HERSHEY_SIMPLEX,
            0.6,
            cv::Scalar(0, 255, 0),
            2
        );
    }

    return result;
}

//保存检测结果
bool saveOutputs(
    const DetectionResult& result,
    const std::string& outputDir
) {
    namespace fs = std::filesystem;

    fs::create_directories(outputDir);

    const std::vector<
        std::pair<std::string, cv::Mat>
    > files = {
        {"red_mask_raw.png", result.redMaskRaw},
        {"red_mask.png", result.redMask},
        {"blue_mask.png", result.blueMask},
        {"light_candidates.png", result.lightDebug},
        {"armor_result.png", result.armorDebug}
    };

    for (const auto& file : files) {

        const fs::path path =
            fs::path(outputDir) / file.first;

        if (file.second.empty()) {
            std::cerr
                << "Empty image: "
                << path << '\n';

            return false;
        }

        if (!cv::imwrite(
                path.string(),
                file.second
            )) {
            std::cerr
                << "Failed to save: "
                << path << '\n';

            return false;
        }

        std::cout
            << "Saved: "
            << path << '\n';
    }

    return true;
}

}









    

