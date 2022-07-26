#include "skybox.h"
#include "cube.h"

using namespace Lynx::Graphics;

Skybox::Skybox(std::shared_ptr<TextureBase> texture) : cube_texture(texture)
{
	const auto core_singleton = Application::GetSingleton();

	cube_mesh = std::make_shared<Cube>();
	cube_shader = core_singleton->GetResourceManager()->LoadShader("res/shaders/cubemap.vert", "res/shaders/cubemap.frag");
}

std::shared_ptr<TextureBase> Skybox::GetTexture()
{
	return cube_texture;
}

std::shared_ptr<Shader> Skybox::GetShader()
{
	return cube_shader;
}

std::shared_ptr<Mesh> Skybox::GetMesh()
{
	return cube_mesh;
}
