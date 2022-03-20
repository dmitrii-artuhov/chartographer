#include "catch2/catch.hpp"
#include "charta/errors.h"
#include "charta/image.h"
#include <iostream>

using namespace charta;

// Create
TEST_CASE("Create image and save to file") {
  BMPImage image;
  CHECK_NOTHROW(image = BMPImage(10, 10, "img1.bmp"));
  CHECK_NOTHROW(image.SaveToFile(""));
}

TEST_CASE("Create image with invalid dimensions") {
  BMPImage image;
  CHECK_THROWS_AS(image = BMPImage(-10, 10, "img1.bmp"), InvalidImageWidth);
  CHECK_THROWS_AS(image = BMPImage(10, -35, "img1.bmp"), InvalidImageHeight);
}

TEST_CASE("Create image with invalid name") {
  BMPImage image;
  CHECK_THROWS_AS(image = BMPImage(10, 35, ""), InvalidImageName);
}

// Delete
TEST_CASE("Delete image successufully") {
  BMPImage image;
  CHECK_NOTHROW(image = BMPImage(10, 10, "img1.bmp"));
  CHECK_NOTHROW(BMPImage::DeleteImage("img1.bmp"));
}

TEST_CASE("Delete image with invalid path") {
  CHECK_THROWS_AS(BMPImage::DeleteImage("do-not-exist.bmp"),
                  UnableToDeleteImage);
}