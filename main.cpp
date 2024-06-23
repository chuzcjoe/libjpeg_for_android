#include <iostream>
#include <turbojpeg.h>
#include <fstream>
#include <vector>
#include <algorithm>

void saveJPEG(const char* filename, const std::vector<unsigned char>& imageBuffer, int width, int height) {
    tjhandle tjInstance = tjInitCompress();
    if (tjInstance == nullptr) {
        std::cerr << "Error: tjInitCompress failed" << std::endl;
        return;
    }

    unsigned char* jpegBuf = nullptr;
    unsigned long jpegSize = 0;

    if (tjCompress2(tjInstance, imageBuffer.data(), width, 0, height, TJPF_RGB, &jpegBuf, &jpegSize, TJSAMP_444, 100, TJFLAG_FASTDCT) != 0) {
        std::cerr << "Error: tjCompress2 failed: " << tjGetErrorStr() << std::endl;
        tjDestroy(tjInstance);
        return;
    }

    std::ofstream outFile(filename, std::ios::binary);
    outFile.write(reinterpret_cast<char*>(jpegBuf), jpegSize);
    outFile.close();

    tjFree(jpegBuf);
    tjDestroy(tjInstance);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_jpeg_file_path> <output_jpeg_file_path>" << std::endl;
        return 1;
    }

    const char* jpegFilePath = argv[1];
    const char* outputFilePath = argv[2];

    std::ifstream file(jpegFilePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << jpegFilePath << std::endl;
        return 1;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(fileSize);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        std::cerr << "Error: Cannot read file " << jpegFilePath << std::endl;
        return 1;
    }

    tjhandle tjInstance = tjInitDecompress();
    if (tjInstance == nullptr) {
        std::cerr << "Error: tjInitDecompress failed" << std::endl;
        return 1;
    }

    int width, height, jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(tjInstance, buffer.data(), buffer.size(), &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        std::cerr << "Error: tjDecompressHeader3 failed: " << tjGetErrorStr() << std::endl;
        tjDestroy(tjInstance);
        return 1;
    }

    std::vector<unsigned char> imageBuffer(width * height * tjPixelSize[TJPF_RGB]);
    if (tjDecompress2(tjInstance, buffer.data(), buffer.size(), imageBuffer.data(), width, 0, height, TJPF_RGB, 0) != 0) {
        std::cerr << "Error: tjDecompress2 failed: " << tjGetErrorStr() << std::endl;
        tjDestroy(tjInstance);
        return 1;
    }

    tjDestroy(tjInstance);

    // Increase brightness
    const int brightnessIncrease = 50;
    for (auto& pixel : imageBuffer) {
        int newPixel = static_cast<int>(pixel) + brightnessIncrease;
        pixel = static_cast<unsigned char>(std::min(newPixel, 255));
    }

    saveJPEG(outputFilePath, imageBuffer, width, height);

    std::cout << "Image processing completed. Brightness increased image saved as '" << outputFilePath << "'" << std::endl;

    return 0;
}

