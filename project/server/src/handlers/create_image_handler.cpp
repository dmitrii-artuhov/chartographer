#include "handlers/create_image_handler.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "utils/utils.h"
#include <iostream>
#include <string>
#include <vector>

using namespace charta;
using namespace Poco::Net;

void CreateImageHandler::handleRequest(HTTPServerRequest &request,
                                       HTTPServerResponse &response) {
  response.setStatus(HTTPResponse::HTTP_CREATED);
  Poco::URI::QueryParameters params = uri_.getQueryParameters();

  std::string response_text = "Create Image:\n";
  for (auto &[key, value] : params) {
    response_text += key + " : " + value + "\n";
  }

  response.setContentType("text/plain");
  response.send() << response_text;
}
