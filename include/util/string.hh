#pragma once

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace fgfwr::util {

/// Split string at whitespaces
inline auto tokenize(const std::string& str) -> std::vector<std::string> {
    std::stringstream stream(str);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(stream),
              std::istream_iterator<std::string>(),
              std::back_inserter(tokens));
    return tokens;
}

} // namespace fgfwr::util
