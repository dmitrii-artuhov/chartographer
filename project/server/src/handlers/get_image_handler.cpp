#include "handlers/get_image_handler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/URI.h"
#include "charta/errors.h"
#include "charta/image.h"
#include "utils/utils.h"
#include <exception>
#include <iostream>
#include <string>
#include <vector>
using namespace charta;
using namespace Poco::Net;

void GetImageHandler::handleRequest(HTTPServerRequest &request,
                                    HTTPServerResponse &response) {
  try {
    Poco::URI::QueryParameters params = uri_.getQueryParameters();
    int width = std::stoi(utils::GetDefaultedQueryValue(params, "width", "0"));
    int height =
        std::stoi(utils::GetDefaultedQueryValue(params, "height", "0"));
    int x = std::stoi(utils::GetDefaultedQueryValue(params, "x", "0"));
    int y = std::stoi(utils::GetDefaultedQueryValue(params, "y", "0"));
    std::string image_name = utils::GetPathSegmentValue(uri_, 1);

    BMPImage stored_image;
    stored_image.LoadFromFile(working_directory_ + "/", image_name + ".bmp");
    std::vector<uint8_t> buffer;
    stored_image.Crop(x, y, width, height).SaveToBuffer(buffer);

    response.setStatus(HTTPResponse::HTTP_OK);
    response.setContentType("image/bmp");
    response.sendBuffer(buffer.data(), buffer.size());
  } catch (const UnableToLoadImage &err) {
    std::cerr << err.what() << std::endl;
    response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND, "Not Found");
    response.send();
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST, "Bad Request");
    response.send();
  }
}
