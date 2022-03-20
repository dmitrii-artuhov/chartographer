#include "catch2/catch.hpp"
#include "charta/errors.h"
#include "charta/image.h"
#include <iostream>

using namespace charta;

TEST_CASE("Create image and save to file") {
  BMPImage image;
  CHECK_NOTHROW(image = BMPImage(10, 10, "img1.bmp"));
  CHECK_NOTHROW(image.SaveToFile(""));
}

TEST_CASE("Create image with invalid dimensions") {
  BMPImage image;
  CHECK_THROWS_WITH(image = BMPImage(-10, 10, "img1.bmp"),
                    "Invalid image width provided: '-10'");
  CHECK_THROWS_WITH(image = BMPImage(10, -35, "img1.bmp"),
                    "Invalid image height provided: '-35'");
}

TEST_CASE("Create image with invalid name") {
  BMPImage image;
  CHECK_THROWS_WITH(image = BMPImage(10, 35, ""),
                    "Invalid image name provided");
}
