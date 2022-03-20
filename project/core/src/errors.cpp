#include "charta/errors.h"
#include <stdexcept>
#include <string>

using namespace charta;

ChartaError::ChartaError(std::string msg)
    : std::runtime_error(std::move(msg)) {}

InvalidImageName::InvalidImageName()
    : ChartaError("Invalid image name provided") {}

InvalidImageWidth::InvalidImageWidth(int width)
    : ChartaError("Invalid image width provided: '" + std::to_string(width) +
                  "'") {}

InvalidImageHeight::InvalidImageHeight(int height)
    : ChartaError("Invalid image height provided: '" + std::to_string(height) +
                  "'") {}

InvalidImageMemoryAllocation::InvalidImageMemoryAllocation(
    const std::string &filename, int size)
    : ChartaError("Could not allocate memory for image: '" + filename +
                  "', with size of: '" + std::to_string(size) + "'") {}

UnableToSaveImage::UnableToSaveImage(const std::string &path)
    : ChartaError("Unable to save image to: '" + path + "'") {}

UnableToDeleteImage::UnableToDeleteImage(const std::string &path)
    : ChartaError("Unable to delete file at '" + path + "'") {}

UnableToLoadImage::UnableToLoadImage(const std::string &path)
    : ChartaError("Unable to load image at '" + path + "'") {}

InvalidImageFormat::InvalidImageFormat()
    : ChartaError("Invalid image format") {}

InvalidImageBounds::InvalidImageBounds()
    : ChartaError("Image subscription does not overlay") {}