// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_EXPERIMENTS_VARIANT_HPP_
#define QUICK_EXPERIMENTS_VARIANT_HPP_

#include <cstddef>
#include <memory>

#include <iostream>
using std::cout;
using std::endl;

namespace quick {
namespace variant_impl {

struct AbstractBaseType {
  virtual ~AbstractBaseType() = default;
};

template<typename T>
struct TypeWrapper: AbstractBaseType {
  T object_;
  template<typename... Args>
  TypeWrapper(Args&&... args): object_(std::forward<Args>(args)...) {}
};


template<std::size_t i, typename... Ts> struct TypeListImpl;

template<std::size_t i> struct TypeListImpl<i> {};

template<std::size_t i, typename T, typename... Ts>
struct TypeListImpl<i, T, Ts...>: TypeListImpl<i+1, Ts...> {};

template<typename... Ts> using TypeList = TypeListImpl<0, Ts...>;

template<std::size_t index, typename T, typename... Ts>
T GetNthTypeFromTypeListFunc(TypeListImpl<index, T, Ts...>& input) {
  return std::declval<T>();
}

template<std::size_t index, typename... Ts>
using GetNthType = decltype(GetNthTypeFromTypeListFunc<index>(
                                  std::declval<TypeList<Ts...>&>()));


}  // namespace variant_impl


template<typename... Ts>
struct variant {
  template<std::size_t index>
  using NthType = typename variant_impl::GetNthType<index, Ts...>;
  template<std::size_t index>
  using NthTypeWrapper = variant_impl::TypeWrapper<NthType<index>>;
  variant() = default;
  variant(const variant& other) {
    this->copy(other);
  }
  variant& operator=(const variant& other) {
    this->copy(other);
    return *this;
  }
  variant(variant&& other) {
    this->move(other);
  }
  variant& operator=(variant&& other) {
    this->move(other);
    return *this;
  }
  template<std::size_t index, typename... Args>
  NthType<index>& at(Args&&... args) {
    if (ptr == nullptr || selected_type_ != index) {
      ptr.reset(new NthTypeWrapper<index>(std::forward<Args>(args)...));
    }
    selected_type_ = index;
    return static_cast<NthTypeWrapper<index>&>(*ptr).object_;
  }

  template<std::size_t index>
  const NthType<index>& at() const {
    if (ptr == nullptr || selected_type_ != index) {
      throw std::runtime_error("[quick::variant]: const access is not allowed "
                               "if corrosponding type is not already set");
    }
    return static_cast<const NthTypeWrapper<index>&>(*ptr).object_;
  }
  void clear() {
    ptr.reset(nullptr);
  }
  bool initialized() const {
    return (ptr != nullptr);
  }
  std::size_t selected_type() const {
    if (ptr == nullptr) {
      return sizeof...(Ts);
    } else {
      return selected_type_;
    }
  }

 private:
  void copy(const variant& other) {
    if (other.ptr == nullptr) {
      ptr.reset(nullptr);
      return;
    }
    copy_impl_type<0, Ts...>(other);
  }
  void move(variant& other) {
    if (other.ptr == nullptr) {
      ptr.reset(nullptr);
      return;
    }
    move_impl_type<0, Ts...>(other);
  }
  template<std::size_t index, typename S, typename... Ss>
  void copy_impl_type(const variant& other) {
    if (index == other.selected_type_) {
      using WT = variant_impl::TypeWrapper<S>;
      if (ptr == nullptr || selected_type_ != index) {
        ptr.reset(new WT(static_cast<const WT&>(*other.ptr).object_));
        selected_type_ = index;
      } else {
        const auto& other_ref = static_cast<const WT&>(*other.ptr).object_;
        static_cast<WT&>(*ptr).object_ = other_ref;
      }
    }
    copy_impl_type<index+1, Ss...>(other);
  }
  template<std::size_t index, typename S, typename... Ss>
  void move_impl_type(variant& other) {
    if (index == other.selected_type_) {
      using WT = variant_impl::TypeWrapper<S>;
      if (ptr == nullptr || selected_type_ != index) {
        ptr.reset(new WT(std::move(static_cast<WT&&>(*other.ptr).object_)));
        selected_type_ = index;
      } else {
        auto&& other_ref = static_cast<WT&&>(*other.ptr).object_;
        static_cast<WT&>(*ptr).object_ = std::move(other_ref);
      }
    }
    move_impl_type<index+1, Ss...>(other);
  }
  template<std::size_t index>
  void copy_impl_type(const variant& other) {}
  template<std::size_t index>
  void move_impl_type(variant& other) {}
  std::unique_ptr<quick::variant_impl::AbstractBaseType> ptr;
  std::size_t selected_type_ = 0;
};



}  // namespace quick

namespace qk = quick;


#endif  // QUICK_EXPERIMENTS_VARIANT_HPP_
