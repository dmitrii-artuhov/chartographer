#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace charta {
class BMPImage {
private:
  std::string GetFullFileLocation(const std::filesystem::path &path) const;

  static int GetPaddingByteCountPerRow(int byte_count_per_row) noexcept;

public:
  std::string filename = "";
  int width = 0;
  int height = 0;
  int size = 0;
  std::vector<uint8_t> data;

  static const int MAX_WIDTH = 20'000;
  static const int MAX_HEIGHT = 50'000;
  static const int BIT_COUNT = 24;
  static const int CHANNELS = 3;
  static const int BYTES_PER_CHANNEL = 8;

  BMPImage() noexcept = default;
  BMPImage(int width_, int height_, std::string filename_);
  void SaveToFile(const std::filesystem::path &path) const;
  void SaveToBuffer(std::vector<uint8_t> &buffer) const;
  static void DeleteImage(const std::filesystem::path &path);
  void LoadFromFile(const std::filesystem::path &path, std::string filename);
  void LoadFromStream(std::istream &stream, std::string filename_);
  void OverlayByImage(const BMPImage &other, int x, int y, int w, int h);
  BMPImage Crop(int x, int y, int w, int h) const;
  void SetName(std::string image_name);
};
} // namespace charta
