#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/URI.h"
#include <string>

namespace charta {
class CreateImageHandler : public Poco::Net::HTTPRequestHandler {
private:
  Poco::URI uri_;
  std::string working_directory_;

public:
  CreateImageHandler(Poco::URI uri, std::string working_directory)
      : uri_(std::move(uri)), working_directory_(std::move(working_directory)) {
  }

  void handleRequest(Poco::Net::HTTPServerRequest &request,
                     Poco::Net::HTTPServerResponse &response) override;
};
} // namespace charta
