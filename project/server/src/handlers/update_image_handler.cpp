#include "handlers/update_image_handler.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "utils/utils.h"

using namespace charta;
using namespace Poco::Net;

void UpdateImageHandler::handleRequest(HTTPServerRequest &request,
                                       HTTPServerResponse &response) {
  response.setStatus(HTTPResponse::HTTP_OK);
  Poco::URI::QueryParameters params = uri_.getQueryParameters();

  std::string response_text =
      "Update Image by ID: " + utils::GetPathSegmentValue(uri_, 1) + "\n";
  for (auto &[key, value] : params) {
    response_text += key + " : " + value + "\n";
  }

  response.setContentType("text/plain");

  response.send() << response_text;
}
