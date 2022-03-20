#include "handlers/update_image_handler.h"
#include "Poco/BinaryReader.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/PartHandler.h"
#include "charta/errors.h"
#include "charta/image.h"
#include "utils/utils.h"
#include <exception>
#include <iostream>
#include <string>

using namespace charta;
using namespace Poco::Net;

namespace {
struct BMPImagePartHandler : PartHandler {
private:
  int width;
  int height;
  int x;
  int y;
  std::string stored_image_name;
  std::string working_directory;

public:
  BMPImagePartHandler(int w, int h, int x, int y, const std::string &name,
                      const std::string working_directory_)
      : width(w), height(h), x(x), y(y), stored_image_name(name),
        working_directory(working_directory_) {}

  void handlePart(const MessageHeader &header, std::istream &stream) override {
    if (stream) {
      BMPImage stored_image;
      stored_image.LoadFromFile(working_directory + "/",
                                stored_image_name + ".bmp");

      BMPImage fragment;
      fragment.LoadFromStream(stream, "fragment.bmp");

      stored_image.OverlayByImage(fragment, x, y, width, height);
      stored_image.SaveToFile(working_directory + "/");
    }
  }
};
} // namespace

void UpdateImageHandler::handleRequest(HTTPServerRequest &request,
                                       HTTPServerResponse &response) {
  try {
    Poco::URI::QueryParameters params = uri_.getQueryParameters();

    int width = std::stoi(utils::GetDefaultedQueryValue(params, "width", "0"));
    int height =
        std::stoi(utils::GetDefaultedQueryValue(params, "height", "0"));
    int x = std::stoi(utils::GetDefaultedQueryValue(params, "x", "0"));
    int y = std::stoi(utils::GetDefaultedQueryValue(params, "y", "0"));
    std::string image_name = utils::GetPathSegmentValue(uri_, 1);

    BMPImagePartHandler handler(width, height, x, y, image_name,
                                working_directory_);
    HTMLForm form(request, request.stream(), handler);

    response.setStatusAndReason(HTTPResponse::HTTP_OK, "OK");
  } catch (const UnableToLoadImage &err) {
    std::cerr << err.what() << std::endl;
    response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND, "Not Found");
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST, "Bad Request");
  }

  response.send();
}
