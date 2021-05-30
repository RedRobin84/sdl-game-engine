#pragma once

#include "enum/SystemType.h"
#include "enum/ProgramTypeEnum.h"
#include "enum/ProgramTypeEnum.h"

#include <unordered_map>
#include <string_view>
#include <memory>

class Asset;
class AssetFactory;
class LTexture;
class System;
class SystemFactory;
class Program;
class ProgramFactory;
class Renderer;

class Registry
{
public:
  Registry();
  std::shared_ptr<LTexture> registerTexture(std::string_view path);
  std::shared_ptr<Renderer> registerRenderer();
  std::shared_ptr<Program> registerProgram(ProgramTypeEnum programType, const std::shared_ptr<Registry> &registry);

  void removeExpired();

private:
  std::unique_ptr<AssetFactory> m_assetFactory;
  std::unique_ptr<SystemFactory> m_systemFactory;
  std::unique_ptr<ProgramFactory> m_programFactory;

  std::unordered_map<ProgramTypeEnum, std::shared_ptr<Program>> m_programMap;
  std::unordered_map<std::string_view, std::weak_ptr<Asset>> m_assetMap;
  std::unordered_map<SystemType, std::weak_ptr<System>> m_systemMap;

  std::shared_ptr<Asset> registerAsset(std::string_view assetName);
  std::shared_ptr<System> registerSystem(SystemType systemType);

};
