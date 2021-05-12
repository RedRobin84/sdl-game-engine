#pragma once

#include <unordered_map>
#include <string_view>
#include <memory>

class Asset;
class LTexture;

class Registry
{
public:
  std::shared_ptr<LTexture> registerTexture(std::string_view path);
  void removeExpired();

private:
  std::unordered_map<std::string_view, std::weak_ptr<Asset>> m_assetMap;

  std::shared_ptr<Asset> registerAsset(std::string_view assetName);
};
