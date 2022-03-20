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
    Poco::URI::QueryParameters params = uri_.getQueryParameters();

    int width = std::stoi(utils::GetDefaultedQueryValue(params, "width", "0"));
    int height =
        std::stoi(utils::GetDefaultedQueryValue(params, "height", "0"));

    std::string uuid = utils::generateUUID().toString();
    std::string image_name = uuid + ".bmp";
    BMPImage image = BMPImage(width, height, image_name);
    image.SaveToFile(working_directory_ + "/");

    response.setStatusAndReason(HTTPResponse::HTTP_CREATED, "Created");
    response.send() << uuid;
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST, "Bad Request");
    response.send();
  }
}
