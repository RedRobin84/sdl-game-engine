#include "Registry.h"
#include "Logger.h"
#include "AssetFactory.h"
#include "LTexture.h"

std::shared_ptr<Asset> Registry::registerAsset(std::string_view assetPath)
{
  if (m_assetMap.contains(assetPath)) {
    if (auto maybeAsset = m_assetMap[assetPath].lock()) {
    Logger::debug("Registry::registerAsset: New shared_ptr for asset: %s. Reference count: %ld", assetPath.data(), m_assetMap[assetPath].use_count());
    return maybeAsset;
    }
    m_assetMap.erase(assetPath);
  }

  std::shared_ptr<Asset> newAsset{AssetFactory::create(assetPath)};
  m_assetMap.emplace(assetPath, newAsset);
  Logger::debug("Registry::registerAsset: New registration for asset: %s. Reference count: %ld", assetPath.data(), newAsset.use_count());
  return newAsset;
}

std::shared_ptr<LTexture> Registry::registerTexture(std::string_view path)
{
  std::shared_ptr<LTexture> maybeTexture = std::dynamic_pointer_cast<LTexture>(registerAsset(path));
  if (maybeTexture) {
    return maybeTexture;
  }
  throw std::bad_cast();
}

void Registry::removeExpired()
{
  Logger::debug("Registry::removeExpired: Removing expired weak_ptrs.");

  std::erase_if(m_assetMap, [](auto &mapRecord) {
    return mapRecord.second.expired();
  });

  Logger::debug("Registry::removeExpired: Asset pool after expired weak_ptrs removal: %zu", m_assetMap.size());
}
