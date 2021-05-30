#pragma once

#include "Registry.h"

#include <memory>
#include <stdexcept>

class Registrable
{

public:
  Registrable(const std::shared_ptr<Registry> &registry) : m_registry(registry) {}
  virtual ~Registrable() = default;

  Registrable() = delete;
  Registrable(const Registrable &) = delete;
  Registrable &operator=(const Registrable &) = delete;
  Registrable &operator=(Program &&) = delete;
  Registrable(Registrable &&) = delete;

  inline std::shared_ptr<Registry> getRegistry()
  {
    if (m_registry) return m_registry;
    throw std::invalid_argument("Registrable::getRegistry: Registry not set.");
  }
  inline void setRegistry(const std::shared_ptr<Registry> &registry)
  {
    m_registry = registry;
  }

private:
  std::shared_ptr<Registry> m_registry;
};
