#pragma once

#include "enum/SystemType.h"

#include <memory>

class System;

class SystemFactory
{
public:
  std::shared_ptr<System> create(SystemType systemType);
};
