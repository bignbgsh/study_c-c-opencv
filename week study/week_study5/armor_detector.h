
#pragma once

#include <opencv2/core.hpp>

#include <array>
#include <string>
#include <vector>

namespace vision {

// 1. 目标颜色
enum class TargetColor {
    Red,
    Blue
};

// 2. 检测参数
struct DetectionParams {

    // 红色（包含偏橙红的灯条）
    cv::Scalar redLow1  = cv::Scalar(0, 75, 145);
    cv::Scalar redHigh1 = cv::Scalar(35, 255, 255);

    cv::Scalar redLow2  = cv::Scalar(170, 75, 145);
    cv::Scalar redHigh2 = cv::Scalar(179, 255, 255);

    // 蓝色范围，为后续扩展保留
    cv::Scalar blueLow  = cv::Scalar(90, 80, 130);
    cv::Scalar blueHigh = cv::Scalar(135, 255, 255);

    // 单根灯条筛选参数
    double minContourArea = 18.0;
    double maxContourArea = 8000.0;

    float minLightLength = 12.0f;
    float minLightWidth = 1.0f;

    float minLightAspect = 2.2f;
    float maxLightAspect = 20.0f;

    double maxLightTiltDeg = 35.0;

    // 两根灯条配对参数
    double maxLengthRatio = 1.6;
    double maxAngleDiffDeg = 15.0;

    double maxYOffsetRatio = 0.7;

    double minSpacingRatio = 1.0;
    double maxSpacingRatio = 5.0;
};

// 3. 单根灯条的数据结构
struct LightBar {

    int id = -1;

    TargetColor color = TargetColor::Red;

    double area = 0.0;

    cv::RotatedRect box;

    cv::Point2f center;
    cv::Point2f top;
    cv::Point2f bottom;

    float length = 0.0f;
    float width = 0.0f;

    double tiltDeg = 0.0;
};

// 4. 装甲板候选的数据结构
struct ArmorCandidate {

    int id = -1;

    TargetColor color = TargetColor::Red;

    int leftLightId = -1;
    int rightLightId = -1;

    cv::Rect boundingBox;

    cv::Point2f center;

    // 顺序：左上、右上、右下、左下
    std::array<cv::Point2f, 4> corners{};

    double score = 0.0;

    bool valid = false;
};

// 5. 完整检测结果
struct DetectionResult {

    cv::Mat redMaskRaw;
    cv::Mat redMask;
    cv::Mat blueMask;

    cv::Mat lightDebug;
    cv::Mat armorDebug;

    std::vector<LightBar> lights;
    std::vector<ArmorCandidate> armors;
};

// 6. 单帧装甲板检测函数
DetectionResult detectArmors(
    const cv::Mat& image,
    const DetectionParams& params
);

// 7. 保存调试图片与最终结果
bool saveOutputs(
    const DetectionResult& result,
    const std::string& outputDir
);

// 8. 颜色名称转换
std::string colorName(TargetColor color);

} // namespace vision
