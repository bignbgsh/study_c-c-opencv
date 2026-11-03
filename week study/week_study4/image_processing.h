#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace vision {

struct Parameters {
    int grayThreshold = 120;

    cv::Scalar hsvLow =
        cv::Scalar(35, 80, 80);

    cv::Scalar hsvHigh =
        cv::Scalar(85, 255, 255);

    double minContourArea = 100.0;
};

struct Detection {
    double area = 0.0;

    cv::Rect boundingBox;
    cv::RotatedRect rotatedBox;
    cv::Point2d center;
};

struct ProcessingResult {
    cv::Mat blurGray;
    cv::Mat maskGray;

    cv::Mat blurColor;
    cv::Mat maskHsv;

    cv::Mat eroded;
    cv::Mat dilated;
    cv::Mat opened;
    cv::Mat closed;

    cv::Mat annotated;

    std::vector<double> candidateAreas;
    std::vector<Detection> detections;
};

// 执行方案一全部图像处理步骤
ProcessingResult process(
    const cv::Mat& input,
    const Parameters& params
);

// 保存九张结果图
bool saveResults(
    const ProcessingResult& results,
    const std::string& outputDir
);

} // namespace vision