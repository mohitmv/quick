// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_DEBUG_STREAM_HPP_
#define QUICK_DEBUG_STREAM_HPP_

#include <iostream>  // NOLINT
#include <map>
#include <utility>
#include <vector>
#include <set>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <string>

#include <quick/type_traits.hpp>

// ToDos()
//  1. Implement DebugStream for std::tuple


namespace quick {

// Not Thread Safe
class DebugStream {
 private:
  using string = std::string;
  using ostringstream = std::ostringstream;

 public:
  std::ostringstream oss;
  DebugStream() {}
  DebugStream(const DebugStream&) = default;
  template<typename T>
  explicit DebugStream(const T& input) {
    (*this) << input;
  }
  template<typename T>
  DebugStream& Consume(const T& input) {
    (*this) << input;
    return *this;
  }
  using BaseStream = std::ostringstream;
  bool is_inline = false;
  uint8_t indentation_space = 2;
  uint32_t depth = 0;

  inline DebugStream& TabSpace() {
    oss << string(depth*indentation_space, ' ');
    return *this;
  }

  DebugStream& PrintChar(char c) {
    oss << c;
    if (c == '\n') {
      TabSpace();
    }
    return *this;
  }

  DebugStream& operator<<(uint8_t c) {
    oss << static_cast<uint16_t>(c);
    return *this;
  }

  DebugStream& operator<<(int8_t c) {
    oss << static_cast<int16_t>(c);
    return *this;
  }

  DebugStream& operator<<(char c) {
    switch (c) {
      case '{':
      case '[':
      case '(':
        return BranchStart(c);
      case '}':
      case ']':
      case ')':
        return BranchEnd(c);
      case '\n':
        oss << c;
        return TabSpace();
      default:
        oss << c;
        break;
    }
    return *this;
  }

  template<typename T>
  std::enable_if_t<(std::is_fundamental<T>::value), DebugStream>&
  operator<<(const T& input) {
    oss << input;
    return *this;
  }

  DebugStream& operator<<(const char* input) {
    char c = input[0];
    if (c == 0) {
      return *this;
    }
    if (input[1] == 0) {
      switch (c) {
        case '{':
        case '[':
        case '(':
          return BranchStart(c);
        case '}':
        case ']':
        case ')':
          return BranchEnd(c);
        default: break;
      }
    }
    for (auto i = input; *i; i++) {
      PrintChar(*i);
    }
    return *this;
  }

  std::string str() const {
    return oss.str();
  }

  DebugStream& operator<<(const std::string& input) {
    for (auto c : input) {
      PrintChar(c);
    }
    return *this;
  }

  template<typename T>
  inline DebugStream& BranchStartInternal(const T& input) {
    oss << input;
    if (not is_inline) {
      oss << "\n";
      depth++;
      TabSpace();
    }
    return *this;
  }

  template<typename T>
  inline DebugStream& BranchEndInternal(const T& input) {
    if (not is_inline) {
      oss << "\n";
      if (depth == 0) {
        throw std::runtime_error("[quick::DebugStream]: Invalid BranchEnd");
      }
      depth--;
      TabSpace();
    }
    oss << input;
    return *this;
  }

  inline DebugStream& BranchStart(const string& s) {
    return BranchStartInternal(s);
  }

  inline DebugStream& BranchStart(const char& s) {
    return BranchStartInternal(s);
  }

  inline DebugStream& BranchEnd(const string& s) {
    return BranchEndInternal(s);
  }

  inline DebugStream& BranchEnd(const char& s) {
    return BranchEndInternal(s);
  }

  template<typename ValueType>
  struct ScopedControlsStruct {
    static_assert(std::is_trivially_destructible<ValueType>::value, "");
    ValueType new_value, original_value;
    ValueType* restore_pointer = nullptr;
    ScopedControlsStruct(ValueType new_value, ValueType* restore_pointer)
          : new_value(new_value),
            original_value(*restore_pointer),
            restore_pointer(restore_pointer) {
      *restore_pointer = new_value;
    }
    ~ScopedControlsStruct() {
      if (restore_pointer != nullptr) {
        *restore_pointer = original_value;
      }
    }
  };

  using SetInlineForThisScope = ScopedControlsStruct<bool>;
  using SetIndentationForThisScope = ScopedControlsStruct<uint8_t>;

  // inline ScopedInlineStruct SetInlineForThisScope(bool value) {
  //   return ScopedInlineStruct(value, &this->is_inline);
  // }

  // inline ScopedIndentationStruct SetIndentationForThisScope(uint8_t value) {
  //   return ScopedIndentationStruct(value, &this->indentation_space);
  // }

  DebugStream& SetInline(bool value) {
    this->is_inline = value;
    return *this;
  }

  DebugStream& SetIndentation(uint8_t value) {
    this->indentation_space = value;
    return *this;
  }
};

namespace detail {
}


template<typename T>
std::enable_if_t<std::is_enum<T>::value, DebugStream>&
operator<<(DebugStream& ds, const T& input) {
  ds << "ENUM-" << static_cast<int32_t>(input);
  return ds;
}


template<typename T>
std::enable_if_t<(quick::is_specialization<T, std::vector>::value ||
                  quick::is_specialization<T, std::list>::value ||
                  quick::is_specialization<T, std::unordered_set>::value ||
                  quick::is_specialization<T, std::set>::value), DebugStream>&
operator<<(DebugStream& ds, const T& input) {
  if (input.size() == 0) {
    ds << "[]";
  } else {
    ds << '[';
    bool is_first_item = true;
    for (const auto& item : input) {
      ds.oss << (is_first_item ? "" : ", ");
      ds << item;
      is_first_item = false;
    }
    ds << ']';
  }
  return ds;
}

template<typename T>
std::enable_if_t<(quick::is_specialization<T, std::map>::value ||
                  quick::is_specialization<T, std::unordered_map>::value),
                 DebugStream>&
operator<<(DebugStream& ds, const T& input) {
  if (input.size() == 0) {
    ds << "{}";
  } else {
    ds << '{';
    bool is_first_item = true;
    for (const auto& item : input) {
      if (not is_first_item) {
        ds.oss << ",";
        if (not ds.is_inline) {
          ds << "\n";
        }
      }
      {
        auto is_inline_prv_value = ds.is_inline;
        ds.is_inline = true;
        ds << item.first;
        ds.is_inline = is_inline_prv_value;
      }
      ds.oss << ": ";
      ds << item.second;
      is_first_item = false;
    }
    ds << '}';
  }
  return ds;
}

template<typename T1, typename T2>
DebugStream& operator<<(DebugStream& ds, const std::pair<T1, T2>& input) {
  ds << "(" << input.first << ", " << input.second << ")";
  return ds;
}

template<typename T>
std::enable_if_t<
  std::is_same<void,
               decltype(
                 std::declval<const T&>().DebugStream(
                   std::declval<quick::DebugStream&>()))>::value,
  DebugStream>&
operator<<(DebugStream& ds, const T& input) {
  ds << "{";
  input.DebugStream(ds);
  ds << "}";
  return ds;
}

}  // namespace quick

namespace qk = quick;

#endif  // QUICK_DEBUG_STREAM_HPP_
