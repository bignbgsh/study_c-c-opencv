#include "image_processing.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>


#include "image_processing.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    // 命令行参数最多四项
    if (argc > 4) {
        std::cerr
            << "Usage: opencv_scheme1 "
            << "[image] [output_dir] "
            << "[normal|empty|half]\n";

        return 1;
    }

    // 1. 输入路径、输出目录和运行模式
    const std::string inputPath =
        argc > 1 ? argv[1] : "images/input.png";

    const std::string outputDir =
        argc > 2 ? argv[2] : "output";

    const std::string mode =
        argc > 3 ? argv[3] : "normal";

    // 检查模式是否合法
    if (mode != "normal" &&
        mode != "empty" &&
        mode != "half") {

        std::cerr
            << "Unknown mode: " << mode
            << " (use normal, empty or half)\n";

        return 1;
    }

    try {
        // 2. 读取彩色原图
        cv::Mat input = cv::imread(
            inputPath,
            cv::IMREAD_COLOR
        );

        // 检查读取是否成功
        if (input.empty()) {
            std::cerr
                << "Cannot read image: "
                << inputPath << '\n';

            std::cerr
                << "Check the filename and "
                << "current working directory.\n";

            return 1;
        }

        // 输出图片尺寸和通道数
        std::cout
            << "Input: " << inputPath << '\n'
            << "Width=" << input.cols
            << ", Height=" << input.rows
            << ", Channels=" << input.channels()
            << '\n';

        // 3. 创建任务参数对象
        vision::Parameters params;

        // 无目标测试模式
        if (mode == "empty") {

            params.hsvLow =
                cv::Scalar(0, 80, 80);

            params.hsvHigh =
                cv::Scalar(10, 255, 255);

        } else if (mode == "half") {

            // 缩小一半测试模式
            cv::Mat smaller;

            cv::resize(
                input,
                smaller,
                cv::Size(),
                0.5,
                0.5,
                cv::INTER_AREA
            );

            input = smaller;

            std::cout
                << "Half size: "
                << input.cols << " x "
                << input.rows << '\n';
        }

        // 4. 执行全部图像处理
        vision::ProcessingResult results =
            vision::process(input, params);

        // 设置浮点数输出格式
        std::cout
            << std::fixed
            << std::setprecision(1);

        // 5. 输出全部候选轮廓面积
        for (
            std::size_t i = 0;
            i < results.candidateAreas.size();
            ++i
        ) {
            std::cout
                << "Candidate " << i + 1
                << " area="
                << results.candidateAreas[i]
                << '\n';
        }

        // 6. 输出有效目标的信息
        for (
            std::size_t i = 0;
            i < results.detections.size();
            ++i
        ) {
            const auto& d = results.detections[i];

            std::cout
                << "Target " << i + 1
                << ": area=" << d.area
                << ", center=("
                << d.center.x << ", "
                << d.center.y << ")"
                << ", bbox=("
                << d.boundingBox.x << ", "
                << d.boundingBox.y << ", "
                << d.boundingBox.width << ", "
                << d.boundingBox.height
                << ")\n";
        }

        // 7. 输出目标总数
        std::cout
            << "Detected targets: "
            << results.detections.size()
            << '\n';

        // 8. 保存全部图片
        if (!vision::saveResults(
                results,
                outputDir
            )) {
            return 1;
        }

        return 0;

    } catch (const cv::Exception& e) {
        std::cerr
            << "OpenCV error: "
            << e.what() << '\n';

    } catch (const std::exception& e) {
        std::cerr
            << "Error: "
            << e.what() << '\n';
    }

    return 1;
}