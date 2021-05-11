#include "Renderable.h"
#include "LTexture.h"
#include "Registry.h"
#include <algorithm>
#include <stdexcept>

Renderable::Renderable(std::string_view texture, Registry &registry)
{
  m_textures.emplace_back(registry.registerTexture(texture));
}

Renderable::Renderable(const std::vector<std::string_view> &textures, Registry &registry)
{
  std::for_each(textures.begin(), textures.end(), [&](std::string_view texturePath) {
    m_textures.emplace_back(registry.registerTexture(texturePath));
  });
}

const LTexture *Renderable::getTexture(std::string_view name)
{
  auto maybe_texture = std::find_if(m_textures.begin(), m_textures.end(), [&name](const auto &texture) { return texture->getName() == name; });
  if (maybe_texture == m_textures.end()) {
    throw std::invalid_argument("Renderable::getTexture: Texture not found.");
  }
  return maybe_texture->get();
}