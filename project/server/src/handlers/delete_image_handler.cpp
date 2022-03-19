#include "handlers/delete_image_handler.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "utils/utils.h"

using namespace charta;
using namespace Poco::Net;

void DeleteImageHandler::handleRequest(HTTPServerRequest &request,
                                       HTTPServerResponse &response) {
  response.setStatus(HTTPResponse::HTTP_OK);

  std::string response_text =
      "Delete image by ID: " + utils::GetPathSegmentValue(uri_, 1);
  response.setContentType("text/plain");
  response.send() << response_text;
}
