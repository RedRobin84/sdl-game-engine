#include "AssetFactory.h"
#include "LTexture.h"
#include "Logger.h"

#include <memory>
#include <stdexcept>

namespace {
constexpr std::string_view TEXTURE_PATH = "/img/";
}

std::shared_ptr<Asset> AssetFactory::create(std::string_view assetPath)
{
  if (assetPath.find(TEXTURE_PATH) != std::string_view::npos) {
    std::shared_ptr<LTexture> newTexture = std::make_shared<LTexture>(assetPath);
    if (!newTexture->loadFromFile(assetPath)) {
      Logger::error("AssetFactory::create: Unable to load texture from file: %s", IMG_GetError());
      throw std::invalid_argument("AssetFactory::create: Unable to create new asset instance from enum.");
    }
    return newTexture;
  }
  throw std::invalid_argument("AssetFactory::create: Unable to create new asset instance from enum.");
}
