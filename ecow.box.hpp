#pragma once
#include "ecow.mod.hpp"
#include "ecow.unit.hpp"

namespace ecow {
class box : public unit {
  std::vector<std::shared_ptr<mod>> m_mods;

  virtual void build_self() const override {
    std::for_each(m_mods.begin(), m_mods.end(), std::mem_fn(&unit::build));
  }
  virtual void create_self_cdb(std::ostream &o) const override {
    for (const auto &u : m_mods) {
      u->create_cdb(o);
    }
  }
  [[nodiscard]] virtual pathset self_objects() const override {
    pathset res{};
    for (const auto &u : m_mods) {
      const auto objs = u->objects();
      std::copy(objs.begin(), objs.end(), std::inserter(res, res.end()));
    }
    return res;
  }

public:
  using unit::unit;

  auto add_mod(const char *name) {
    auto res = create<mod>(name);
    m_mods.push_back(res);
    return res;
  }

  virtual void visit(features f, strmap &out) const override {
    unit::visit(f, out);
    std::for_each(m_mods.begin(), m_mods.end(),
                  [f, &out](auto &u) { u->visit(f, out); });
  }

  virtual void recurse_wsdeps(wsdeps::map_t &res) const override {
    unit::recurse_wsdeps(res);
    for (const auto &u : m_mods) {
      u->recurse_wsdeps(res);
    }
  }

  [[nodiscard]] virtual strset link_flags() const override {
    strset res{unit::link_flags()};
    for (const auto &u : m_mods) {
      const auto fws = u->link_flags();
      std::copy(fws.begin(), fws.end(), std::inserter(res, res.end()));
    }
    return res;
  }

  [[nodiscard]] virtual pathset resources() const override {
    pathset res{unit::resources()};
    for (const auto &u : m_mods) {
      const auto fws = u->resources();
      std::copy(fws.begin(), fws.end(), std::inserter(res, res.end()));
    }
    return res;
  }
};
} // namespace ecow
