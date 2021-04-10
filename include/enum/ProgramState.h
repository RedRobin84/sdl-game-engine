#pragma once

enum class ProgramState : unsigned int {
  TERMINUS,
  RUNNING,
  ADD,
  REMOVE,
  REPLACE,
  RESET
};
