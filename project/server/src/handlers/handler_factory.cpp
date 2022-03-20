#include "handlers/handler_factory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"
#include "handlers/create_image_handler.h"
#include "handlers/delete_image_handler.h"
#include "handlers/get_image_handler.h"
#include "handlers/hello_handler.h"
#include "handlers/not_found_handler.h"
#include "handlers/update_image_handler.h"

#include <iostream>
#include <string>
#include <vector>

using namespace charta;
using namespace Poco::Net;

Poco::Net::HTTPRequestHandler *
HandlerFactory::createRequestHandler(const HTTPServerRequest &request) {
  Poco::URI uri{request.getURI()};

  // Get segments
  std::vector<std::string> uriSegments;
  uri.getPathSegments(uriSegments);

  if (uriSegments.size() == 1U && uriSegments[0] == "hello" &&
      request.getMethod() == "GET") {
    return new HelloHandler{uri};
  } else if (uriSegments.size() == 1U && uriSegments[0] == "chartas" &&
             request.getMethod() == "POST") {
    return new CreateImageHandler{uri, working_directory_};
  } else if (uriSegments.size() == 2U && uriSegments[0] == "chartas" &&
             request.getMethod() == "POST") {
    return new UpdateImageHandler{uri, working_directory_};
  } else if (uriSegments.size() == 2U && uriSegments[0] == "chartas" &&
             request.getMethod() == "GET") {
    return new GetImageHandler{uri, working_directory_};
  } else if (uriSegments.size() == 2U && uriSegments[0] == "chartas" &&
             request.getMethod() == "DELETE") {
    return new DeleteImageHandler{uri, working_directory_};
  }

  return new NotFoundHandler{};
}
