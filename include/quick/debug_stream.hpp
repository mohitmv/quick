// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)
//


#ifndef QUICK_DEBUG_STREAM_HPP_
#define QUICK_DEBUG_STREAM_HPP_

#include <iostream>  // NOLINT
#include <map>
#include <utility>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <sstream>


namespace quick {
namespace detail {


}

class DebugStream: public std::ostringstream {
 private:
  using string = std::string;
  using ostringstream = std::ostringstream;
 public:
  bool is_inline = false;
  uint8_t indentation_space = 2;
  uint32_t depth = 0;
  enum BranchControls {BRANCH_START, BRANCH_END};

  struct SetInLine {
    bool original_value, current_value;
    DebugStream* linked_debug_stream = nullptr;
    SetInLine(bool current_value): current_value(current_value) {}
    ~SetInLine() {
      if (linked_debug_stream != nullptr) {
        linked_debug_stream->is_inline = original_value;
      }
    }
  };

  struct SetIndentationSpace {
    uint8_t original_value, current_value;
    DebugStream* linked_debug_stream = nullptr;
    SetIndentationSpace(uint8_t current_value): current_value(current_value) {}
    ~SetIndentationSpace() {
      if (linked_debug_stream != nullptr) {
        linked_debug_stream->indentation_space = original_value;
      }
    }
  };

  inline void TabSpace() {
    ostringstream::operator<<(string(depth*indentation_space, ' ').c_str());
  }

  DebugStream& operator<<(const char* input) {
    return this->operator<<(std::string(input));
  }

  DebugStream& operator<<(const std::string& input) {
    for (auto c: input) {
      std::ostringstream::operator<<(c);
      if (c == '\n') {
        TabSpace();
      }
    }
    return *this;
  }

  DebugStream& operator<<(BranchControls bc) {
    switch (bc) {
      case BRANCH_START: {
        std::ostringstream::operator<<("{\n");
        depth++;
        TabSpace();
        return *this;
      }
      case BRANCH_END: {
        std::ostringstream::operator<<("\n");
        depth--;
        TabSpace();
        std::ostringstream::operator<<("}\n");
        TabSpace();
        return *this;
      }
    }
  }
};


}

namespace qk = quick;

#endif  // QUICK_DEBUG_STREAM_HPP_
