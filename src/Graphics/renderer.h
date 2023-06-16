#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <stack>
#include <glm/glm.hpp>
#include "mesh.h"
#include "camera.h"
#include "lynx_common.h"

namespace Lynx {

	class Entity;
	class Scene;
	class Skybox;


	class Material : public Resource {
	public:
		Material() : ambient(glm::vec3(1.0f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3(1.0f)) {}
		Material(std::shared_ptr<TextureBase> texture) : ambient(glm::vec3(1.0f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3(1.0f)), texture(texture) {}
		Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, std::shared_ptr<Shader> shader) : ambient(a), diffuse(d), specular(s), shader(shader) {}
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		std::shared_ptr<Shader> shader;
		std::shared_ptr<TextureBase> texture;
		std::shared_ptr<TextureBase> texture_diffuse;
		std::shared_ptr<TextureBase> texture_specular;
	};

	class PointLight {
	public:
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Ambient, Diffuse, Specular = glm::vec3(1.0f);
		float Constant, Linear, Quadratic = 0.0f;
	};

	class DirectionalLight {
	public:
		glm::vec3 Direction = glm::vec3(0.0f);
		glm::vec3 Ambient, Diffuse, Specular = glm::vec3(1.0f);
		float Intensity = 2.0f;
	};
	
	/**
	 * @brief Abstract base class for an object or entity, that is meant to be rendered.
	 *
	 */
	class LYNXENGINE_API Renderable {
	public:
		/**
		 * @brief Construct a new Renderable object
		 *
		 */
		Renderable() {}
		Renderable(Material mat) : m_material(mat) {}
		Renderable(MeshHndlPtr mesh, Material mat) :
			m_mesh(mesh), m_material(mat) {}

		/**
		 * @brief Adds a mesh to the renderable object
		 *
		 * @param mesh
		 */
		void SetMesh(MeshHndlPtr mesh) { m_mesh = mesh; }

		MeshHndlPtr GetMesh() { return m_mesh; }
		/**
		 * @brief Sets the Material
		 *
		 * @param mat
		 */
		void SetMaterial(Material mat) { m_material = mat; }

		Material GetMaterial() { return m_material; }

	protected:
		MeshHndlPtr m_mesh;
		Material m_material;
	};

	typedef std::shared_ptr<Renderable> RenderHndlPtr;

	/**
	 * @brief Abstract base renderer class
	 *
	 */
	class LYNXENGINE_API Renderer {
	protected:
		typedef struct {
			Material mat;
			MeshHndlPtr mesh;
			glm::mat4 transform;
		} render_queue_obj;

		std::stack<render_queue_obj> m_renderQueue;
		std::vector<PointLight> m_pointLights;
		Camera* m_camera;
		DirectionalLight m_directionalLight;

	public:
		Renderer() = default;

		virtual ~Renderer() = default;

		virtual void Initialise() = 0;

		void SetCamera(Camera* camera) { m_camera = camera; }

		Camera* GetCamera() { return m_camera; }

		virtual void Update() = 0;

		virtual void Shutdown() = 0;

		virtual void PushRender(Entity* ent) = 0;
		virtual void PushRender(Renderable* renderable, glm::mat4 modelMatrix) = 0;

		virtual void PushLight(PointLight& light) = 0;
		virtual void SetDirectionalLight(DirectionalLight& light) = 0;

		virtual void SetSkybox(std::shared_ptr<Skybox> skybox) = 0;

		virtual void Render() = 0;
	};

}

#endif // RENDERER_H
