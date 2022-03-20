#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "charta/image.h"
#include "charta/errors.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

using namespace charta;

int BMPImage::GetPaddingByteCountPerRow(int byte_count_per_row) noexcept {
  return (4 - (byte_count_per_row % 4)) % 4;
}

std::string
BMPImage::GetFullFileLocation(const std::filesystem::path &path) const {
  return path.string() + filename;
}

BMPImage::BMPImage(int width_, int height_, std::string filename_) {
  if (width_ <= 0 || width_ > BMPImage::MAX_WIDTH) {
    throw InvalidImageWidth(width_);
  }

  if (height_ <= 0 || height_ > BMPImage::MAX_HEIGHT) {
    throw InvalidImageHeight(height_);
  }

  if (filename_.empty()) {
    throw InvalidImageName();
  }

  filename = std::move(filename_);
  width = width_;
  height = height_;
  size = width * height * BMPImage::CHANNELS;
  try {
    data.resize(size, 0U);
  } catch (const std::bad_alloc &) {
    throw InvalidImageMemoryAllocation(filename, size);
  }
}

void BMPImage::SaveToFile(const std::filesystem::path &path) const {
  std::string location = GetFullFileLocation(path);
  if (!stbi_write_bmp(location.c_str(), width, height, BMPImage::CHANNELS,
                      data.data())) {
    throw UnableToSaveImage(location);
  }
}

void BMPImage::SaveToBuffer(std::vector<uint8_t> &buffer) const {
    // stbi_write_bmp_core
};

void BMPImage::DeleteImage(const std::filesystem::path &path) {
  try {
    if (!std::filesystem::remove(path)) {
      throw UnableToDeleteImage(path.string());
    }
  } catch (const std::filesystem::filesystem_error &) {
    throw UnableToDeleteImage(path.string());
  }
}

void BMPImage::LoadFromFile(const std::filesystem::path &path,
                            std::string filename_) {
  int received_channels;
  std::string location = path.string() + filename_;
  auto *ptr = stbi_load(location.c_str(), &width, &height, &received_channels,
                        BMPImage::CHANNELS);

  if (ptr == nullptr) {
    throw UnableToLoadImage(location);
  }

  if (received_channels != BMPImage::CHANNELS) {
    throw InvalidImageFormat();
  }

  filename = std::move(filename_);
  size = width * height * BMPImage::CHANNELS;

  try {
    data.resize(size);
    std::memcpy(data.data(), ptr, size);

  } catch (const std::bad_alloc &) {
    throw InvalidImageMemoryAllocation(path, size);
  } catch (...) {
    throw UnableToLoadImage(path);
  }
}

void BMPImage::SetName(std::string image_name) {
  if (image_name.find("/") != std::string::npos) {
    throw InvalidImageName();
  }

  filename = std::move(image_name);
}

void BMPImage::LoadFromStream(std::istream &stream, std::string filename_) {
  std::vector<uint8_t> buffer;
  uint8_t byte;
  while (stream >> byte) {
    buffer.push_back(byte);
  }

  int received_channels;
  auto *ptr =
      stbi_load_from_memory(buffer.data(), buffer.size(), &width, &height,
                            &received_channels, BMPImage::CHANNELS);

  if (ptr == nullptr) {
    throw UnableToLoadImage("stream");
  }

  if (received_channels != BMPImage::CHANNELS) {
    throw InvalidImageFormat();
  }

  filename = std::move(filename_);
  size = width * height * BMPImage::CHANNELS;

  try {
    data.resize(size);
    std::memcpy(data.data(), ptr, size);

  } catch (const std::bad_alloc &) {
    throw InvalidImageMemoryAllocation("stream", size);
  } catch (...) {
    throw UnableToLoadImage("stream");
  }
}

void BMPImage::OverlayByImage(const BMPImage &fragment, int x, int y, int w,
                              int h) {
  if (x + w <= 0 || y + h <= 0 || x >= width || y >= height) {
    throw InvalidImageBounds();
  }
  int start_pos_x = std::max(x, 0);
  int start_pos_y = std::max(y, 0);

  int end_pos_x = std::min(width, x + w);
  int end_pos_y = std::min(height, y + h);

  int offset_x = x < 0 ? -x : 0;
  int offset_y = y < 0 ? -y : 0;

  int bytes_per_row = width * BMPImage::CHANNELS;
  int fragment_bytes_per_row = fragment.width * BMPImage::CHANNELS;
  int bytes_per_pixel = BMPImage::CHANNELS;

  for (int i = start_pos_y; i < end_pos_y; i++) {
    for (int j = start_pos_x; j < end_pos_x; j++) {
      int pos = i * bytes_per_row + j * bytes_per_pixel;
      int fragment_pos = (offset_y + i - start_pos_y) * fragment_bytes_per_row +
                         (offset_x + j - start_pos_x) * (bytes_per_pixel);
      for (int k = 0; k < bytes_per_pixel; k++) {
        data[pos + k] = fragment.data[fragment_pos + k];
      }
    }
  }
}

BMPImage BMPImage::Crop(int x, int y, int w, int h) const {
  if (x + w <= 0 || y + h <= 0 || x >= width || y >= height) {
    throw InvalidImageBounds();
  }

  BMPImage image(w, h, "crop-" + filename);

  int offset_x = x < 0 ? -x : 0;
  int offset_y = y < 0 ? -y : 0;

  int bytes_per_row = width * BMPImage::CHANNELS;
  int other_bytes_per_row = w * BMPImage::CHANNELS;
  int max_row_index = std::min(height, y + h);
  int max_index =
      std::min(x + w, width - (x >= 0 ? x : 0)) * BMPImage::CHANNELS;

  x = std::max(x, 0);
  y = std::max(y, 0);

  for (int row_index = y; row_index < max_row_index; row_index++) {
    for (int ptr = row_index * bytes_per_row + x * BMPImage::CHANNELS,
             other_idx = (row_index - y + offset_y) * other_bytes_per_row +
                         offset_x * BMPImage::CHANNELS,
             i = 0;
         i < max_index; ptr++, other_idx++, i++) {
      image.data[other_idx] = data[ptr];
    }
  }

  return image;
}

// change stored image to the cropped one
// void crop(int x, int y, int w, int h) {
//   int image_height = bmp_image.bmp_info_header.image_height;
//   int image_width = bmp_image.bmp_info_header.image_width;
//   int bit_count = bmp_image.bmp_info_header.bit_count;

//   // change image origin to match th `raw_pixel_data`
//   y = image_height - (y + h);

//   // check for valid crop bounds
//   if (y < 0 || y + h > image_height || x < 0 || x + w > image_width || h <= 0
//   ||
//       w <= 0) {
//     throw invalid_crop_bounds();
//   }

//   // do the crop
//   int byte_count_per_pixel = bit_count / 8;

//   int byte_count_per_row = image_width * byte_count_per_pixel;
//   int padding_byte_count_per_row =
//       BMP::get_padding_byte_count_per_row(byte_count_per_row);
//   int row_length = byte_count_per_row + padding_byte_count_per_row;

//   int new_byte_count_per_row = w * byte_count_per_pixel;
//   int new_padding_byte_count_per_row =
//       BMP::get_padding_byte_count_per_row(new_byte_count_per_row);
//   int new_row_length = new_byte_count_per_row +
//   new_padding_byte_count_per_row;

//   std::vector<std::uint8_t> new_raw_pixel_data;
//   try {
//     new_raw_pixel_data.resize(new_row_length * h);
//   } catch (const std::bad_alloc &) {
//     throw insufficient_memory();
//   }

//   int idx = 0;
//   for (auto row_index = y; row_index < y + h; row_index++) {
//     for (int ptr = row_index * row_length + byte_count_per_pixel * x;
//          ptr < row_index * row_length + byte_count_per_pixel * (x + w);
//          ptr++, idx++) {
//       new_raw_pixel_data[idx] = bmp_image.raw_pixel_data[ptr];
//     }

//     for (auto _ = 0; _ < new_padding_byte_count_per_row; _++, idx++) {
//       new_raw_pixel_data[idx] = 0U;
//     }
//   }

//   // change the stored BMP image
//   bmp_image.file_header.file_size =
//       sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) +
//       static_cast<std::uint32_t>(new_raw_pixel_data.size());

//   bmp_image.bmp_info_header.image_height = h;
//   bmp_image.bmp_info_header.image_width = w;

//   bmp_image.bmp_info_header.size_image =
//       static_cast<std::uint32_t>(new_raw_pixel_data.size());

//   bmp_image.raw_pixel_data = new_raw_pixel_data;
// }