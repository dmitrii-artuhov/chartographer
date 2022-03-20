#include "handlers/delete_image_handler.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "charta/image.h"
#include "utils/utils.h"
#include <iostream>

using namespace charta;
using namespace Poco::Net;

void DeleteImageHandler::handleRequest(HTTPServerRequest &request,
                                       HTTPServerResponse &response) {
  try {
    std::string image_name = utils::GetPathSegmentValue(uri_, 1) + ".bmp";
    BMPImage::DeleteImage(working_directory_ + "/" + image_name);
    response.setStatusAndReason(HTTPResponse::HTTP_OK, "OK");
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND, "Not Found");
  }

  response.send();
}
