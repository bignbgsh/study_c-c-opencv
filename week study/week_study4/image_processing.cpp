#include "image_processing.h"

#include <opencv2/geometry/2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace vision {

ProcessingResult process(
    const cv::Mat& input,
    const Parameters& params
) {
    if (input.empty()|| input.channels() != 3){
        throw std::invalid_argument(
            "Input image must be non-empty and have 3 channels.");

}

    if (params.minContourArea < 0){
        throw std::invalid_argument(
            "Minimum contour area must be positive.");
    }

    ProcessingResult r;

    cv::Mat gray;
     
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray, r.blurGray, cv::Size(5, 5), 0);

    cv::threshold(
        r.blurGray,
        r.maskGray,
        params.grayThreshold,
        255,
        cv::THRESH_BINARY_INV
    );

    //HSV 颜色分割方案
    cv::medianBlur(
        input,
        r.blurColor,
        5
    );

    cv::Mat hsv;
    
    cv::cvtColor(
        r.blurColor,
        hsv,
        cv::COLOR_BGR2HSV
    );

    cv::inRange(
        hsv,
        params.hsvLow,
        params.hsvHigh,
        r.maskHsv
    );

    //创建形态学核

    const cv::Mat kernel3 = 
        cv::getStructuringElement(
            cv::MORPH_RECT,
            cv::Size(3, 3)
    );

     const cv::Mat kernel7 =
        cv::getStructuringElement(
            cv::MORPH_RECT,
            cv::Size(7, 7)
     );

     //创建独立形态学操作
     cv::erode(
        r.maskHsv,
        r.eroded,
        kernel3
    );

    cv::dilate(
        r.maskHsv,
        r.dilated,
        kernel3
    );

    cv::morphologyEx(
        r.maskHsv,
        r.opened,
        cv::MORPH_OPEN,
        kernel3
    );

    cv::morphologyEx(
        r.maskHsv,
        r.closed,
        cv::MORPH_CLOSE,
        kernel7
    );

    //从开运算中提取轮廓
    cv::Mat contourInput = r.opened.clone();

    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(
        contourInput,
        contours,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE
    );

    //画框
    r.annotated = input.clone();

    for (const auto& contour : contours) {

        const double area = 
            cv::contourArea(contour);
        r.candidateAreas.push_back(area);

        if (area < params.minContourArea) {
            continue;
        }

       const cv::Moments moments = 
            cv::moments(contour);
        
        if (moments.m00 == 0.0) {
            std::cerr
                << "Warning: contour has m00=0, "
                << "skipping center computation\n";

            continue;
        }

        Detection found;

        found.area = area;

        found.boundingBox =
            cv::boundingRect(contour);

        found.rotatedBox =
            cv::minAreaRect(contour);

        found.center = cv::Point2d(
            moments.m10 / moments.m00,
            moments.m01 / moments.m00
        );

        r.detections.push_back(found);

        //绘制绿色矩形框
        cv::rectangle(
            r.annotated,
            found.boundingBox,
            cv::Scalar(0, 200, 0),
            2
        );

        //绘制紫红色旋转矩形框
        cv::Point2f corners[4];

        found.rotatedBox.points(corners);

        for (int i = 0; i < 4; ++i) {
            const cv::Point p1(
                cvRound(corners[i].x),
                cvRound(corners[i].y)
            );

            const cv::Point p2(
                cvRound(corners[(i + 1) % 4].x),
                cvRound(corners[(i + 1) % 4].y)
            );

            cv::line(
                r.annotated,
                p1,
                p2,
                cv::Scalar(255, 0, 255),
                2
            );

            // 11. 绘制中心点
        const cv::Point center(
            cvRound(found.center.x),
            cvRound(found.center.y)
        );

        // 先画黑色外圈，增强可见度
        cv::circle(
            r.annotated,
            center,
            5,
            cv::Scalar(0, 0, 0),
            cv::FILLED
        );

        // 再画红色实心中心点
        cv::circle(
            r.annotated,
            center,
            3,
            cv::Scalar(0, 0, 255),
            cv::FILLED
        );
        }              
        
}
return r;
}
// 12. 保存全部图片
bool saveResults(
    const ProcessingResult& r,
    const std::string& outputDir
) {
    namespace fs = std::filesystem;

    fs::create_directories(outputDir);

    const std::vector<
        std::pair<std::string, cv::Mat>
    > files = {
        {"fixed_blur_gray.png", r.blurGray},
        {"fixed_mask_gray.png", r.maskGray},
        {"fixed_blur_color.png", r.blurColor},
        {"fixed_mask_hsv.png", r.maskHsv},
        {"fixed_eroded.png", r.eroded},
        {"fixed_dilated.png", r.dilated},
        {"fixed_opened.png", r.opened},
        {"fixed_closed.png", r.closed},
        {"fixed_result.png", r.annotated}
    };

    for (const auto& file : files) {

        const fs::path path =
            fs::path(outputDir) / file.first;

        if (file.second.empty() ||
            !cv::imwrite(
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


