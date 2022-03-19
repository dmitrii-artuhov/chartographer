#include "handlers/hello_handler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/URI.h"
#include "charta/hello.h"
#include <iomanip>

using namespace charta;
using namespace Poco::Net;

namespace {
std::string GetDefaultedQueryValue(Poco::URI::QueryParameters query,
                                   std::string_view needle,
                                   std::string_view default_value) {
  for (const auto &[key, value] : query) {
    if (key == needle) {
      return value;
    }
  }

  return std::string{default_value};
}
} // namespace

void HelloHandler::handleRequest(HTTPServerRequest &request,
                                 HTTPServerResponse &response) {
  auto name =
      GetDefaultedQueryValue(uri_.getQueryParameters(), "name", "world");

  response.setContentType("text/plain");
  response.send() << make_hello(name);
}
