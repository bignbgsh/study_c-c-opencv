#include "image_basic.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <array>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    // 检查命令行参数
    if (argc > 4 ||
        (argc == 4 &&
         std::string(argv[3]) != "--no-gui")) {

        std::cerr
            << "Usage: opencv_basic "
            << "[image_path] [output_dir] [--no-gui]\n";

        return 1;
    }

    // 输入图片与输出目录
    const fs::path inputPath =
        (argc >= 2) ? argv[1] : "input.jpg";

    const fs::path outputDir =
        (argc >= 3) ? argv[2] : "output";

    const bool noGui = (argc == 4);

    try {
        // 第一步：读取原图
        const cv::Mat image = cv::imread(
            inputPath.string(),
            cv::IMREAD_COLOR
        );

        if (image.empty()) {
            std::cerr
                << "Cannot read image: "
                << inputPath << '\n';

            std::cerr
                << "Check file name and "
                << "current working directory.\n";

            return 1;
        }

        // 输出图片基本信息
        std::cout
            << "Input: " << inputPath << '\n';

        std::cout
            << "Width=" << image.cols
            << ", Height=" << image.rows
            << ", Channels=" << image.channels()
            << '\n';

        // 第二步：创建输出目录
        fs::create_directories(outputDir);

        if (!fs::is_directory(outputDir)) {
            std::cerr
                << "Output path is not a directory: "
                << outputDir << '\n';

            return 1;
        }

        // 第三步：执行图片处理
        const cv::Mat gray =
            image_basic::toGray(image);

        const cv::Mat hsv =
            image_basic::toHsv(image);

        const cv::Rect region =
            image_basic::centereRoi(image);

        const cv::Mat roi =
            image_basic::cropChecked(image, region);

        const cv::Mat resized =
            image_basic::resizeHalf(image);

        const cv::Mat result =
            image_basic::drawAnnotations(image);

        // 输出 ROI 和缩放后的尺寸
        std::cout
            << "ROI: x=" << region.x
            << ", y=" << region.y
            << ", width=" << roi.cols
            << ", height=" << roi.rows
            << '\n';

        std::cout
            << "Resized: "
            << resized.cols
            << " x "
            << resized.rows
            << '\n';

        // 第四步：整理全部输出文件
        const std::array<
            std::pair<fs::path, cv::Mat>, 5
        > outputs = {{

            {outputDir / "gray.png", gray},
            {outputDir / "hsv.png", hsv},
            {outputDir / "roi.png", roi},
            {outputDir / "resized.png", resized},
            {outputDir / "result.png", result}

        }};

        // 防止意外覆盖输入图片
        const fs::path normalizedInput =
            fs::absolute(inputPath).lexically_normal();

        for (const auto& item : outputs) {
            if (
                fs::absolute(item.first).lexically_normal()
                    == normalizedInput ||
                (
                    fs::exists(item.first) &&
                    fs::equivalent(item.first, inputPath)
                )
            ) {
                std::cerr
                    << "Refusing to overwrite original image: "
                    << item.first << '\n';

                return 1;
            }
        }

        // 第五步：逐张保存
        for (const auto& item : outputs) {
            if (!image_basic::saveImage(
                    item.first,
                    item.second
                )) {
                return 1;
            }
        }

        std::cout
            << "All five output images "
            << "saved successfully.\n";

        // 第六步：显示图片
        if (!noGui) {
            cv::imshow("Original", image);
            cv::imshow("Result", result);

            std::cout
                << "Click an image window, "
                << "then press any key to exit.\n";

            cv::waitKey(0);
            cv::destroyAllWindows();
        }

        return 0;

    } catch (const fs::filesystem_error& e) {
        std::cerr
            << "Filesystem error: "
            << e.what() << '\n';

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