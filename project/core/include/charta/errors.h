#pragma once

#include <stdexcept>
#include <string>

namespace charta {
struct ChartaError : std::runtime_error {
  explicit ChartaError(std::string msg);
};

struct InvalidImageName : ChartaError {
  explicit InvalidImageName();
};

struct InvalidImageWidth : ChartaError {
  explicit InvalidImageWidth(int width);
};

struct InvalidImageHeight : ChartaError {
  explicit InvalidImageHeight(int height);
};

struct InvalidImageMemoryAllocation : ChartaError {
  explicit InvalidImageMemoryAllocation(const std::string &filename, int size);
};

struct UnableToSaveImage : ChartaError {
  explicit UnableToSaveImage(const std::string &path);
};

} // namespace charta