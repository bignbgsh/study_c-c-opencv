#pragma once



#include <opencv2/core.hpp>
#include <filesystem>

namespace image_basic{

    cv::Mat toGray(const cv::Mat& source);

    cv::Mat toHsv(const cv::Mat& source);

    cv::Rect centereRoi(
        const cv::Mat& source,
        int preferredWidth = 200,
        int preferredHeight = 120
    );

    cv::Mat cropChecked(
        const cv::Mat& source,
        const cv::Rect& region
    );

    cv::Mat resizeHalf(const cv::Mat& source);

    cv::Mat drawAnnotations(const cv::Mat& source);

    bool saveImage(
        const std::filesystem::path& filename,
        const cv::Mat& image
    );
    






}