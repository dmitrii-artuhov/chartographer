// #pragma once
#ifndef TEST_
#define TEST_
#include "Poco/URI.h"
#include <string>

namespace charta::utils {
inline std::string GetPathSegmentValue(Poco::URI &uri, int index = 0) {
  std::vector<std::string> pathComponents;
  uri.getPathSegments(pathComponents);
  return pathComponents[index];
}
} // namespace charta::utils
#endif