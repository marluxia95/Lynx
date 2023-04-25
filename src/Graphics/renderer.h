#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <stack>
#include <glm/glm.hpp>
#include "mesh.h"
#include "camera.h"
#include "lighting.h"
#include "material.h"
#include "lynx_common.h"

namespace Lynx {

class Entity;
class Scene;

namespace Graphics {

    class Skybox;

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

}

#endif // RENDERER_H
