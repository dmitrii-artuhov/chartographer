#include "handlers/create_image_handler.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/UUIDGenerator.h"
#include "charta/image.h"
#include "utils/utils.h"
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace charta;
using namespace Poco::Net;

void CreateImageHandler::handleRequest(HTTPServerRequest &request,
                                       HTTPServerResponse &response) {
  try {
    response.setStatus(HTTPResponse::HTTP_CREATED);
    Poco::URI::QueryParameters params = uri_.getQueryParameters();

    int width, height;
    std::string response_text = "Create Image:\n";
    for (auto &[key, value] : params) {
      response_text += key + " : " + value + "\n";
      if (key == "width") {
        width = std::stoi(value);
      } else if (key == "height") {
        height = std::stoi(value);
      }
    }

    std::string image_name = utils::generateUUID().toString() + ".bmp";
    BMPImage image = BMPImage(width, height, image_name);
    image.SaveToFile("");

    response.setContentType("text/plain");
    response.send() << response_text;
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
  }
}
