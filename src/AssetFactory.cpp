#include "AssetFactory.h"
#include "LTexture.h"

#include <memory>
#include <stdexcept>

namespace {
constexpr std::string_view TEXTURE_PATH = "/img/";
}

std::shared_ptr<Asset> AssetFactory::create(std::string_view assetPath)
{
  if (assetPath.find(TEXTURE_PATH) != std::string_view::npos) {
    return std::make_shared<LTexture>(assetPath);
  }
  throw std::invalid_argument("AssetFactory::create: Unable to create new asset instance from enum.");
}