#include "image_basic.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace image_basic{

cv::Mat toGray(const cv::Mat& source){
    cv::Mat gray;
    cv::cvtColor(source,gray,cv::COLOR_BGR2GRAY);
    return gray;

}

cv::Mat toHsv(const cv::Mat& source){
    cv::Mat hsv;
    cv::cvtColor(source,hsv,cv::COLOR_BGR2HSV);
    return hsv;

}

cv::Rect centereRoi(
        const cv::Mat& source,
        int preferredWidth ,
        int preferredHeight ){
    
        if(source.empty()){
            throw std::invalid_argument(
                "ROI error:image is empty"
            );
        }

        if (preferredWidth <= 0 || preferredHeight <= 0){

        throw std::invalid_argument(
            "ROI error: width/height must be positive"
        );
        }
        const int width = 
            std::min(preferredHeight,source.cols);

        const int height = 
            std::min(preferredHeight, source.rows);
        
        const int x = (source.cols - width) / 2;
        const int y = (source.rows - height) / 2;

        return cv::Rect(x,y,width,height);

    }

cv::Mat cropChecked(
    const cv::Mat& source,
    const cv::Rect& region
) {
    if (source.empty() ||
        region.x < 0 ||
        region.y < 0 ||
        region.width <= 0 ||
        region.height <= 0 ||
        region.width > source.cols ||
        region.height > source.rows ||
        region.x > source.cols - region.width ||
        region.y > source.rows - region.height) {

        throw std::out_of_range(
            "ROI error: rectangle is outside the image"
        );
    }

    return source(region).clone();
}

cv::Mat resizeHalf(const cv::Mat& source){
    if(source.empty()){
        throw std::invalid_argument(
            "Resize error: image is empty"
        );
    }

    const int newWidth = 
        std::max(1,(source.cols+1)/2);
    const int newHeight = 
        std::max(1,(source.rows+1)/2);

    cv::Mat resized;
    
    cv::resize(
        source,resized,
        cv::Size(newWidth,newHeight),
        0.0,
        0.0,
        cv::INTER_AREA

    );
    return resized;

}

cv::Mat drawAnnotations(const cv::Mat& source) {
    if (source.empty()) {
        throw std::invalid_argument(
            "Drawing error: image is empty"
        );
    }

    cv::Mat result = source.clone();

    const int width = source.cols;
    const int height = source.rows;
    const int minSide = std::min(width, height);

    const int thickness =
        std::max(1, minSide / 250);

    // 绿色矩形
    const int left = width / 8;
    const int top = height / 8;

    cv::rectangle(
        result,
        cv::Rect(
            left,
            top,
            width - 2 * left,
            height - 2 * top
        ),
        cv::Scalar(0, 255, 0),
        thickness
    );

    cv::line(
        result,
        cv::Point(0, height - 1),
        cv::Point(width - 1, 0),
        cv::Scalar(255, 0, 0),
        thickness
    );

     // 红色填充圆点
    const cv::Point center(
        width / 2,
        height / 2
    );

    const int radius =
        std::max(1, minSide / 12);

    cv::circle(
        result,
        center,
        radius,
        cv::Scalar(0, 0, 255),
        cv::FILLED
    );

    // 白色英文文字
    const int textX = std::min(10, width - 1);

    const int textY = std::min(
        height - 1,
        std::max(12, height / 10)
    );

    const double fontScale =
        std::max(0.3, minSide / 600.0);

    cv::putText(
        result,
        "OpenCV Basic",
        cv::Point(textX, textY),
        cv::FONT_HERSHEY_SIMPLEX,
        fontScale,
        cv::Scalar(255, 255, 255),
        thickness
    );

    return result;
}
// 7. 保存图片并检查结果
bool saveImage(
    const std::filesystem::path& filename,
    const cv::Mat& image
) {
    try {
        if (image.empty()) {
            std::cerr
                << "Save failed: empty image: "
                << filename << '\n';

            return false;
        }

        if (!cv::imwrite(filename.string(), image)) {
            std::cerr
                << "Save failed: "
                << filename << '\n';

            return false;
        }

        std::cout
            << "Saved: "
            << filename << '\n';

        return true;

    } catch (const cv::Exception& e) {
        std::cerr
            << "Save failed: "
            << filename << '\n'
            << e.what() << '\n';

        return false;
    }
}

}



















