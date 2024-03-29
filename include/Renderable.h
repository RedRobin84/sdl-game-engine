#pragma once

#include <vector>
#include <memory>
#include <string_view>

class LTexture;
class Registry;

class Renderable
{
public:
  Renderable(std::string_view texture, Registry &registry);
  Renderable(const std::vector<std::string_view> &textures, Registry &registry);
  const LTexture *getTexture(std::string_view name);

private:
  std::vector<std::shared_ptr<LTexture>> m_textures;
  friend class Renderer;
  friend struct RenderableTest;
};
