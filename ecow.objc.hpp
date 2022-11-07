#pragma once

#include "ecow.unit.hpp"

namespace ecow {
class objc : public unit {
public:
  using unit::unit;

  void add_framework(const std::string &name) {
    add_link_flag("-framework " + name);
  }

  void build() override {
    if (target_supports(impl::target::objective_c))
      unit::build();
  }

  [[nodiscard]] virtual strvec objects() const override {
    return target_supports(impl::target::objective_c) ? unit::objects()
                                                      : strvec{};
  }

  [[nodiscard]] virtual strset link_flags() const override {
    return target_supports(impl::target::objective_c) ? unit::link_flags()
                                                      : strset{};
  }
};
} // namespace ecow
