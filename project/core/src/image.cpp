
#include "charta/image.h"
#include "charta/errors.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <string>

using namespace charta;

std::string BMPImage::GetFullFileLocation(const std::filesystem::path &path) {
  return path.string() + filename;
}

BMPImage::BMPImage(int width_, int height_, std::string filename_) {
  if (width_ < 0 || width_ > BMPImage::MAX_WIDTH) {
    throw InvalidImageWidth(width_);
  }

  if (height_ < 0 || height_ > BMPImage::MAX_HEIGHT) {
    throw InvalidImageHeight(height_);
  }

  if (filename_.empty()) {
    throw InvalidImageName();
  }

  filename = std::move(filename_);
  width = width_;
  height = height_;
  size = width * height * BMPImage::BIT_COUNT;

  try {
    data.resize(size, 0U);
  } catch (const std::bad_alloc &) {
    throw InvalidImageMemoryAllocation(filename, size);
  }
}

void BMPImage::SaveToFile(std::filesystem::path path) {
  std::string location = GetFullFileLocation(path);
  if (!stbi_write_bmp(location.c_str(), width, height, BMPImage::CHANNELS,
                      data.data())) {
    throw UnableToSaveImage(location);
  }
}

// bool BMPImage::LoadFromFile(std::filesystem::path path) {
//   int received_channels;
//   data.data() = stbi_load(GetFullFileLocation(path).c_str(), &width, &height,
//                           &received_channels, BMPImage::CHANNELS);
// }