#pragma once

#include <string_view>

class Asset
{
public:
  Asset() = default;
  Asset(std::string_view path) : m_path(path) {}
  virtual ~Asset() = default;

private:
  std::string_view m_path;
};