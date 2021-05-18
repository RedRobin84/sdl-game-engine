#include "Registry.h"
#include "Logger.h"
#include "AssetFactory.h"
#include "ProgramFactory.h"
#include "SystemFactory.h"
#include "LTexture.h"
#include "Renderer.h"

Registry::Registry() : 
                        m_assetFactory(std::make_unique<AssetFactory>()), 
                        m_programFactory(std::make_unique<ProgramFactory>()), 
                        m_systemFactory(std::make_unique<SystemFactory>()) {}


std::shared_ptr<Asset> Registry::registerAsset(std::string_view assetPath)
{
  if (m_assetMap.contains(assetPath)) {
    if (auto maybeAsset = m_assetMap[assetPath].lock()) {
      Logger::debug("Registry::registerAsset: New shared_ptr for asset: %s. Reference count: %ld", assetPath.data(), m_assetMap[assetPath].use_count());
      return maybeAsset;
    }
    m_assetMap.erase(assetPath);
  }

  std::shared_ptr<Asset> newAsset{ m_assetFactory->create(assetPath) };
  m_assetMap.emplace(assetPath, newAsset);
  Logger::debug("Registry::registerAsset: New registration for asset: %s. Reference count: %ld", assetPath.data(), newAsset.use_count());
  return newAsset;
}

std::shared_ptr<System> Registry::registerSystem(SystemType systemType)
{
  if (m_systemMap.contains(systemType)) {
    if (auto maybeSystem = m_systemMap[systemType].lock()) {
    Logger::debug("Registry::registerSystem: New shared_ptr for system: %s. Reference count: %ld", getSystemName(systemType)), m_assetMap[systemType].use_count());
    return maybeSystem;
    }
    m_systemMap.erase(systemType);
  }

  std::shared_ptr<System> newSystem{ m_systemFactory->create(systemType) };
  m_assetMap.emplace(systemType, newSystem);
  Logger::debug("Registry::registerSystem: New registration for system: %s. Reference count: %ld", getSystemName(systemType)), newSystem.use_count());
  return newSystem;
}

std::shared_ptr<LTexture> Registry::registerTexture(std::string_view path)
{
  std::shared_ptr<LTexture> maybeTexture = std::dynamic_pointer_cast<LTexture>(registerAsset(path));
  if (maybeTexture) {
    return maybeTexture;
  }
  throw std::bad_cast();
}

std::shared_ptr<Renderer> Registry::registerRenderer(SystemType systemType)
{
  std::shared_ptr<Renderer> maybeRenderer = std::dynamic_pointer_cast<Renderer>(registerSystem(systemType));
  if (maybeRenderer) {
    return maybeRenderer;
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
