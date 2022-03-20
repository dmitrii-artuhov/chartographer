#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <filesystem>
#include <string>
#include <vector>

namespace charta {
class BMPImage {
private:
  std::string filename = "";
  int width = 0;
  int height = 0;
  int size = 0;
  std::vector<uint8_t> data;

  std::string GetFullFileLocation(const std::filesystem::path &path);

public:
  static const int MAX_WIDTH = 20'000;
  static const int MAX_HEIGHT = 50'000;
  static const int BIT_COUNT = 24;
  static const int CHANNELS = 3;
  static const int BYTES_PER_CHANNEL = 8;

  BMPImage() = default;
  BMPImage(int width_, int height_, std::string filename_);
  void SaveToFile(std::filesystem::path path);
  // bool LoadFromFile(std::filesystem::path path);
};
} // namespace charta
