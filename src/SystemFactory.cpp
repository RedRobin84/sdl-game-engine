#include "SystemFactory.h"
#include "Renderer.h"

#include <stdexcept>

std::shared_ptr<System> SystemFactory::create(SystemType systemType)
{
  switch (systemType) {
  case SystemType::RENDERER:
    return std::make_shared<Renderer>();
  default:
    throw std::invalid_argument("SystemFactory::create: No case for system type found.");
  }
}