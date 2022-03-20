#include "catch2/catch.hpp"
#include "charta/errors.h"
#include "charta/image.h"
#include <iostream>

using namespace charta;

// Create
TEST_CASE("Create image and save to file") {
  BMPImage image;
  CHECK_NOTHROW(image = BMPImage(3, 2, "img1.bmp"));
  CHECK_NOTHROW(image.SaveToFile("./"));
}

TEST_CASE("Create image with invalid dimensions") {
  BMPImage image;
  CHECK_THROWS_AS(image = BMPImage(-10, 10, "img2.bmp"), InvalidImageWidth);
  CHECK_THROWS_AS(image = BMPImage(10, -35, "img2.bmp"), InvalidImageHeight);
}

TEST_CASE("Create image with invalid name") {
  BMPImage image;
  CHECK_THROWS_AS(image = BMPImage(10, 35, ""), InvalidImageName);
}

// Delete
TEST_CASE("Delete image successufully") {
  BMPImage image;
  CHECK_NOTHROW(image = BMPImage(10, 10, "img3.bmp"));
  CHECK_NOTHROW(image.SaveToFile("./"));
  CHECK_NOTHROW(BMPImage::DeleteImage("./img3.bmp"));
}

TEST_CASE("Delete image with invalid path") {
  CHECK_THROWS_AS(BMPImage::DeleteImage("do-not-exist.bmp"),
                  UnableToDeleteImage);
}

// Loading from file
TEST_CASE("Load from file") {
  BMPImage image;
  CHECK_NOTHROW(image.LoadFromFile("../../.data/", "small-one.bmp"));
  CHECK_NOTHROW(image.SaveToFile("../../.data/"));
}

TEST_CASE("Invalid load from file") {
  BMPImage image;
  CHECK_THROWS_AS(image.LoadFromFile("do-not-exist.bmp", "new-small-one.bmp"),
                  UnableToLoadImage);
}

// Overlay
TEST_CASE("Overlay by other image") {
  BMPImage image;
  CHECK_NOTHROW(image.LoadFromFile("../../.data/", "small-one.bmp"));
  CHECK_NOTHROW(image.SetName("overlayed-small-one.bmp"));
  CHECK_NOTHROW(
      image.OverlayByImage(BMPImage(10, 10, "fragment"), -8, -9, 10, 10));
  CHECK_NOTHROW(image.SaveToFile("../../.data/"));
}

TEST_CASE("Overlay big image with small one") {
  BMPImage small, big;
  small.LoadFromFile("../../.data/", "small-one.bmp");
  small.SetName("small-fragment");

  big.LoadFromFile("../../.data/", "big-one.bmp");
  big.SetName("big-overlayed.bmp");

  big.OverlayByImage(small, 29, 19, small.width, small.height);
  big.SaveToFile("../../.data/");
}

// Crop
TEST_CASE("Crop image") {
  BMPImage image;
  CHECK_NOTHROW(image.LoadFromFile("../../.data/", "small-one.bmp"));
  CHECK_NOTHROW(image.SetName("cropped-small-one.bmp"));
  CHECK_NOTHROW(image.Crop(0, 1, 3, 2).SaveToFile("../../.data/"));
}

TEST_CASE("Crop another image") {
  BMPImage image;
  CHECK_NOTHROW(image.LoadFromFile("../../.data/", "big-one.bmp"));
  CHECK_NOTHROW(image.SetName("cropped-big-one.bmp"));
  CHECK_NOTHROW(image.Crop(0, 0, 3, 2).SaveToFile("../../.data/"));
  std::vector<uint8_t> buffer;
  image.Crop(0, 0, 3, 2).SaveToBuffer(buffer);
  for (int byte : buffer) {
    std::cout << byte << ' ';
  }
  std::cout << std::endl;
}

// Save to buffer
// TEST_CASE("Save to buffer") {
//   BMPImage image;
//   image.LoadFromFile("../../.data/", "small-one.bmp");
//   std::vector<uint8_t> buffer;
//   image.SaveToBuffer(buffer);
// for (int byte : buffer) {
//   std::cout << byte << ' ';
// }
// std::cout << std::endl;
// }