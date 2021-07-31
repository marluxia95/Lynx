#ifndef GLVERTEXARRAY_H
#define GLVERTEXARRAY_H

#include <memory>


namespace Lynx::Graphics::OpenGL {

    class GLVertexArray {
        public:
            GLVertexArray();
            ~GLVertexArray();
            void Bind();
            void Unbind();
            virtual void SetElementBuffer(std::shared_ptr<ElementBuffer> elementBuffer);
            virtual std::shared_ptr<ElementBuffer> GetElementBuffer();
            unsigned int VAO_ID;

        private:
            std::shared_ptr<ElementBuffer> m_elementBuffer;

    };

}

#endif