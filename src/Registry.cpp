#include "Registry.h"
#include "Logger.h"
#include "AssetFactory.h"
#include "LTexture.h"

namespace {
constexpr size_t SINGLE = 1;
}

std::shared_ptr<Asset> Registry::registerAsset(std::string_view assetPath)
{
  if (m_assetMap.contains(assetPath)) {
    std::shared_ptr<Asset> existingAsset{ m_assetMap[assetPath] };
    Logger::debug("Registry::registerAsset: New shared_ptr for asset: %s. Reference count: %ld", assetPath.data(), m_assetMap[assetPath].use_count());
    return existingAsset;
  }

  std::shared_ptr<Asset> newShared = createNewAssetRegistration(assetPath);
  Logger::debug("Registry::registerAsset: New shared_ptr for asset: %s. Total asset pool size: %zu", assetPath.data(), m_assetMap.size());
  return newShared;
}

std::shared_ptr<LTexture> Registry::registerTexture(std::string_view path)
{
  std::shared_ptr<LTexture> maybeTexture = std::dynamic_pointer_cast<LTexture>(registerAsset(path));
  if (maybeTexture) {
    return maybeTexture;
  }
  throw std::bad_cast();
}

void Registry::removeOrphans()
{
  Logger::debug("Registry::removeOrphans: Removing assets with single ref count.");

  std::erase_if(m_assetMap, [](auto &mapRecord) {
    return mapRecord.second.use_count() == SINGLE;
  });

  Logger::debug("Registry::removeOrphans: Asset pool after orphan removal: %zu", m_assetMap.size());
}

std::shared_ptr<Asset> Registry::createNewAssetRegistration(std::string_view assetPath)
{
  auto [newAsset, inserted] = m_assetMap.emplace(assetPath, AssetFactory::create(assetPath));
  Logger::debug("Registry::createNewAssetRegistration: New registration for asset: %s. Reference count: %ld", newAsset->first.data(), newAsset->second.use_count());
  return newAsset->second;
}
