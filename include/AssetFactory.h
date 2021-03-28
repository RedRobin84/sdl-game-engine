#pragma once

#include <memory>
#include <string_view>

class Asset;

class AssetFactory
{
public:
  static std::shared_ptr<Asset> create(std::string_view assetPath);
};