
#include "armor_detector.h"

#include <opencv2/imgcodecs.hpp>

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    // 1. 检查命令行参数数量
    if (argc > 4) {
        std::cerr
            << "Usage: armor_detect "
            << "[image_path] [output_dir] "
            << "[normal|empty]\n";

        return 1;
    }

    // 2. 设置输入图片、输出目录和模式
    const std::string imagePath =
        argc >= 2 ? argv[1] : "images/outpost_frame.png";

    const std::string outputDir =
        argc >= 3 ? argv[2] : "output";

    const std::string mode =
        argc >= 4 ? argv[3] : "normal";

    if (mode != "normal" && mode != "empty") {
        std::cerr << "Unknown mode: " << mode << '\n';
        return 1;
    }

    try {

        // 3. 读取原始图片
        const cv::Mat input = cv::imread(
            imagePath,
            cv::IMREAD_COLOR
        );

        // 判断是否读取成功
        if (input.empty()) {
            std::cerr
                << "Cannot read image: "
                << imagePath << '\n';

            return 1;
        }

        // 输出原图信息
        std::cout
            << "Input: " << imagePath << '\n'
            << "Width: " << input.cols << '\n'
            << "Height: " << input.rows << '\n'
            << "Channels: " << input.channels()
            << '\n';

        // 4. 设置检测参数
        vision::DetectionParams params;

        // 无目标测试
        if (mode == "empty") {
            params.minContourArea = 1e9;
        }

        // 5. 执行单帧装甲板检测
        const vision::DetectionResult results =
            vision::detectArmors(input, params);

        std::cout
            << std::fixed
            << std::setprecision(1);

        // 6. 输出灯条候选数量
        std::cout
            << "\nLight candidates: "
            << results.lights.size()
            << '\n';

        // 7. 输出每根灯条的信息
        for (const auto& light : results.lights) {

            std::cout
                << "Light " << light.id
                << ", color=" << vision::colorName(light.color)
                << ", area=" << light.area
                << ", length=" << light.length
                << ", width=" << light.width
                << ", tilt=" << light.tiltDeg
                << ", center=("
                << light.center.x << ", "
                << light.center.y << ")\n";
        }

        // 8. 输出装甲板数量
        std::cout
            << "\nArmor candidates: "
            << results.armors.size()
            << '\n';

        if (results.armors.empty()) {
            std::cout
                << "No armor candidates found.\n";
        }

        // 9. 输出装甲板详细信息
        for (const auto& armor : results.armors) {

            std::cout
                << "\nArmor " << armor.id << '\n'
                << "color="
                << vision::colorName(armor.color)
                << '\n'
                << "valid="
                << std::boolalpha << armor.valid
                << '\n'
                << "lights=("
                << armor.leftLightId << ", "
                << armor.rightLightId << ")\n"
                << "score=" << armor.score << '\n';

            std::cout
                << "bbox=("
                << armor.boundingBox.x << ", "
                << armor.boundingBox.y << ", "
                << armor.boundingBox.width << ", "
                << armor.boundingBox.height << ")\n";

            std::cout
                << "center=("
                << armor.center.x << ", "
                << armor.center.y << ")\n";

            // 输出四个角点
            for (std::size_t i = 0;
                 i < armor.corners.size();
                 ++i) {

                std::cout
                    << "corner[" << i << "]=("
                    << armor.corners[i].x << ", "
                    << armor.corners[i].y
                    << ")\n";
            }
        }

        // 10. 保存所有处理结果
        if (!vision::saveOutputs(
                results,
                outputDir
            )) {

            return 1;
        }

        std::cout
            << "\nProcessing finished.\n";

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
