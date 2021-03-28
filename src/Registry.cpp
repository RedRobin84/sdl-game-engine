#include "Registry.h"
#include "Logger.h"
#include "AssetFactory.h"

namespace {
constexpr size_t SINGLE = 1;
}

std::shared_ptr<Asset> Registry::registerAsset(std::string_view assetPath)
{
  if (m_assetMap.contains(assetPath)) {
    std::shared_ptr<Asset> existingAsset{ m_assetMap[assetPath] };
    Logger::debug("Registry::registerAsset: New shared_ptr for asset: %s. Reference count: %ld", assetPath, m_assetMap[assetPath].use_count());
    return existingAsset;
  }

  std::shared_ptr<Asset> newShared = createNewAssetRegistration(assetPath);
  Logger::debug("Registry::registerAsset: New shared_ptr for asset: %s. Reference count: %ld", assetPath, newShared.use_count());
  return newShared;
}

void Registry::removeOrphans()
{
  std::erase_if(m_assetMap, [](auto &mapRecord) {
    return mapRecord.second.use_count() == SINGLE;
  });
}

std::shared_ptr<Asset> Registry::createNewAssetRegistration(std::string_view assetPath)
{
  auto [newAsset, inserted] = m_assetMap.emplace(assetPath, AssetFactory::create(assetPath));
  Logger::debug("Registry::createNewAssetRegistration: New registration for asset: %s. Reference count: %ld", newAsset->first, newAsset->second.use_count());
  return newAsset->second;
}