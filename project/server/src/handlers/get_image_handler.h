#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/URI.h"
#include <string>

namespace charta {
class GetImageHandler : public Poco::Net::HTTPRequestHandler {
private:
  Poco::URI uri_;
  std::string working_directory_;

public:
  GetImageHandler(Poco::URI uri, std::string working_directory)
      : uri_(std::move(uri)), working_directory_(std::move(working_directory)) {
  }

  void handleRequest(Poco::Net::HTTPServerRequest &request,
                     Poco::Net::HTTPServerResponse &response) override;
};
} // namespace charta
