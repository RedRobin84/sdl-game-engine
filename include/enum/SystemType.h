#pragma once

enum class SystemType {
    RENDERER
};

static const char *getSystemName(SystemType systemType)
{
  switch (systemType) {
  case SystemType::RENDERER:
    return "RENDERER";
  default:
    return "ERROR_UNKNOWN";
  }
}