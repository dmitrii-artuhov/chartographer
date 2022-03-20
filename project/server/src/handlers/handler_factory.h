#pragma once
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include <string>

namespace charta {
class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
  std::string working_directory_;

public:
  HandlerFactory(const std::string &working_directory)
      : working_directory_(working_directory) {}
  Poco::Net::HTTPRequestHandler *
  createRequestHandler(const Poco::Net::HTTPServerRequest &request) override;
};
} // namespace charta
