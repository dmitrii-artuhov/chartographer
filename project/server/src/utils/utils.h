// #pragma once
#ifndef TEST_
#define TEST_
#include "Poco/URI.h"
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include <string>

namespace charta::utils {
inline std::string GetPathSegmentValue(Poco::URI &uri, int index = 0) {
  std::vector<std::string> pathComponents;
  uri.getPathSegments(pathComponents);
  return pathComponents[index];
}

inline static Poco::UUIDGenerator getUUIDGenerator() {
  return Poco::UUIDGenerator();
}

inline Poco::UUID generateUUID() { return getUUIDGenerator().create(); }

inline std::string GetDefaultedQueryValue(Poco::URI::QueryParameters query,
                                          std::string_view needle,
                                          std::string_view default_value) {
  for (const auto &[key, value] : query) {
    if (key == needle && !value.empty()) {
      return value;
    }
  }

  return std::string{default_value};
}
} // namespace charta::utils
#endif