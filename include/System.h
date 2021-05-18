#pragma once

#include "enum/SystemType.h"

class System {

public:
  System() = default;
  System(SystemType systemType) : m_systemType(systemType) {}
  virtual ~System() = default;

private:
  SystemType m_systemType;

};
