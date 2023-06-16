#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/logger.h"
#include "Core/assert.h"
#include "gl_graphics_api.h"
#include "shader.h"
#include "mesh.h"

GLenum checkerror_(const char* f, int l)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        const char* error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        log_error("GLerror in %s ( %d ) : %s", f, l, error);
    }
    return errorCode;
}

namespace Lynx {

    std::map<ShaderType, GLenum> shader_type_map = {
        {SHADER_NULL,       NULL                },
        {SHADER_VERTEX,     GL_VERTEX_SHADER    },
        {SHADER_FRAGMENT,   GL_FRAGMENT_SHADER  },
        {SHADER_GEOMETRY,   GL_GEOMETRY_SHADER  },
        {SHADER_COMPUTE,    GL_COMPUTE_SHADER   }
    };

    std::map<TextureType, GLenum> texture_type_map = {
        {TEXTURE_1D,                GL_TEXTURE_1D},
        {TEXTURE_1D_ARRAY,          GL_TEXTURE_1D_ARRAY},
        {TEXTURE_2D,                GL_TEXTURE_2D},
        {TEXTURE_2D_ARRAY,          GL_TEXTURE_2D_ARRAY},
        {TEXTURE_3D,                GL_TEXTURE_3D},
        {TEXTURE_CUBE_MAP,          GL_TEXTURE_CUBE_MAP},
        {TEXTURE_CUBE_MAP_ARRAY,    GL_TEXTURE_CUBE_MAP_ARRAY}
    };

	GLShaderProgram::GLShaderProgram()
    {
        id = glCreateProgram(); 
#ifdef API_DEBUG
        log_debug("[GL] Initialized shader program with ID %d", id);
#endif
    }

    GLShaderProgram::~GLShaderProgram()
    {
        glDeleteProgram(id);
#ifdef API_DEBUG
        log_debug("[GL] Deleted shader program with ID %d", id);
#endif
    }

    void GLShaderProgram::AttachShader(unsigned int shaderID)
    {
        glAttachShader(id, shaderID);
#ifdef API_DEBUG
        log_debug("[GL] Attached shader program with ID %d to %d", shaderID, id);
#endif
    }

    void GLShaderProgram::Use()
    {
        glUseProgram(id);
#ifdef API_DEBUG
        log_debug("[GL] Used shader program with ID %d", id);
#endif
    }
    
    bool GLShaderProgram::Link()
    {
        glLinkProgram(id);
		int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(id, MAX_ERR_BUFSIZE, NULL, error_log);
			log_error("Error while linking shaders! : %s", error_log);
		}
#ifdef API_DEBUG
        log_debug("[GL] Linked shader program with ID %d; Success : %d", id, success);
#endif
        glCheckError();
		return (bool)success;
    }	

	GLVertexBuffer::GLVertexBuffer(const void* data, unsigned int size)
    {
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		log_debug("[GL] Initialized vertex buffer");
	}

    GLVertexBuffer::GLVertexBuffer(std::vector<Vertex>* vertices)
    {
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(vertices->at(0)), GL_STATIC_DRAW);
		log_debug("[GL] Initialized vertex buffer");
	}

    GLVertexBuffer::GLVertexBuffer(std::vector<Vertex>* vertices, MeshType type)
	{
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &vertices->at(0), GL_STATIC_DRAW);
		Configure(type);
		log_debug("[GL] Initialized vertex buffer");
	}

	GLVertexBuffer::GLVertexBuffer(unsigned int size)
	{
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
		log_debug("[GL] Initialized vertex buffer");
	}

	GLVertexBuffer::GLVertexBuffer()
	{
		glGenBuffers(1, &VBO_ID);
		log_debug("[GL] Initialized vertex buffer");
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &VBO_ID);
	}

	void GLVertexBuffer::SetData(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	void GLVertexBuffer::DynamicDraw(bool dynamic_draw)
	{
		dynamic = dynamic_draw;
	}

	void GLVertexBuffer::AddData(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void GLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID); 
	}

	void GLVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
	}

	void GLVertexBuffer::Configure(MeshType type)
	{
		switch( type ){

			case MESH_2D_SPRITE:
				// Vertex attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float),  (void*)0); 
	    		glEnableVertexAttribArray(0);
				break;
			case MESH_3D:
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
				glEnableVertexAttribArray(0);
				break;
			case MESH_3D_NORMAL:
				// Vertex attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
				glEnableVertexAttribArray(0);
				// Normal attribute
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
				glEnableVertexAttribArray(1);
				break;
			case MESH_3D_TEXTURED:
				// Vertex attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	    		glEnableVertexAttribArray(0);
	    		// Texture attribute
	    		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
	    		glEnableVertexAttribArray(2);
				break;
			case MESH_3D_TEXTURED_NORMAL:
				// Vertex attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
				glEnableVertexAttribArray(0);
				// Normal attribute
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
				glEnableVertexAttribArray(1);
				// Texture attribute
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
				glEnableVertexAttribArray(2);
				break;
		}
	}
 
    GLElementBuffer::GLElementBuffer(const void* indices, unsigned int indexNumber){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNumber, indices, GL_STATIC_DRAW);
		log_debug("[GL] Initialized element buffer");
    }

    GLElementBuffer::GLElementBuffer(std::vector<unsigned int>* indices){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &(indices->at(0)), GL_STATIC_DRAW );
		log_debug("[GL] Initialized element buffer");
    }

    GLElementBuffer::GLElementBuffer(unsigned int size){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW );
		log_debug("[GL] Initialized element buffer");
    }

    GLElementBuffer::GLElementBuffer()
    {
        glGenBuffers(1, &ID);
		log_debug("[GL] Initialized element buffer");
    }

    GLElementBuffer::~GLElementBuffer()
	{
        glDeleteBuffers(1, &ID);
    }

	void GLElementBuffer::DynamicDraw(bool dynamic_draw)
	{
		dynamic = dynamic_draw;
	}

	void GLElementBuffer::SetData(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

    void GLElementBuffer::AddData(const void* data, unsigned int size)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);  
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
    }

    void GLElementBuffer::Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }

    void GLElementBuffer::Unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    }

	GLVertexArray::GLVertexArray(){
		glGenVertexArrays(1, &VAO_ID);
		glBindVertexArray(VAO_ID);
		log_debug("[GL] Initialized vertex array");
	}

	GLVertexArray::~GLVertexArray(){
		glDeleteVertexArrays(1, &VAO_ID);
		log_debug("[GL] Destroyed vertex array");
	}

	void GLVertexArray::Bind(){
		glBindVertexArray(VAO_ID);
	}

	void GLVertexArray::Unbind(){
		glBindVertexArray(0);
	}

    void GLRendererAPI::Init()
    {
        glewExperimental = true;
        LYNX_ASSERT(glewInit() == GLEW_OK, "Unable to initialize GLEW");

        glEnable(GL_DEPTH_TEST);
	
		glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
    	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// Enable face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glFrontFace(GL_CW);  

        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        log_info("GLRendererAPI: Loaded API ( %s )", glGetString(GL_VERSION));
        glCheckError();
    }

    void GLRendererAPI::Clear(glm::vec4 color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLRendererAPI::SetViewport(uint32_t width, uint32_t height)
    {
        glViewport(0, 0, width, height);
    }
    
    unsigned int GLRendererAPI::GenTexture()
    {
        unsigned int tex;
        glGenTextures(1, &tex);
        glCheckError();
        return tex;
    }

    void GLRendererAPI::LoadTexture(TextureType type, unsigned char* data, int width, int height, bool useDefaults, int format)
    {
        LYNX_ASSERT(data != NULL, "Invalid data");
        unsigned int t_type = texture_type_map[type];
        if(useDefaults) {
            glTexParameteri(t_type, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
            glTexParameteri(t_type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(t_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(t_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        
        glTexImage2D(t_type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(t_type);
        glCheckError();
    }

    void GLRendererAPI::SetTextureParameter(unsigned int param, unsigned int value)
    {
        glTexParameteri(GL_TEXTURE_2D, param, value);
        glCheckError();
    }

    void GLRendererAPI::UseTexture(unsigned int id)
    {
        glActiveTexture(GL_TEXTURE0 + id);
        glCheckError();
    }

    void GLRendererAPI::BindTexture(unsigned int tex)
    {
        glBindTexture(GL_TEXTURE_2D, tex);
        glCheckError();
    }

    void GLRendererAPI::BindTexture(TextureType type, unsigned int tex)
    {
        glBindTexture(texture_type_map[type], tex);
        glCheckError();
    }

    void GLRendererAPI::DrawIndexed(int n)
    {
        glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, (void*)0);
        glCheckError();
    }

    void GLRendererAPI::DrawArrays(int n)
    {
        glDrawArrays(GL_TRIANGLES, 0, n);
        glCheckError();
    }

    void GLRendererAPI::CheckErrors(const char* f, int l)
    {
        checkerror_(f,l);
    }

    unsigned int GLRendererAPI::CompileShader(const char* shaderSource, ShaderType type)
    {
        glCheckError();
        unsigned int shaderID = glCreateShader(shader_type_map[type]);
        glShaderSource(shaderID, 1, &shaderSource, NULL);
        glCompileShader(shaderID);
        glCheckError();

        // Check shader errors
        int success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shaderID, MAX_ERR_BUFSIZE, NULL, error_log);
            log_error("Error while compiling shader! : %s", error_log);
            return -1;
        }
        return shaderID;
    }

    void GLRendererAPI::SetShaderUniformBool(int location, bool value)
    {
        glUniform1i(location, value);
    }

    void GLRendererAPI::SetShaderUniformInt(int location, int value)
    {
        glUniform1i(location, value);
    }

    void GLRendererAPI::SetShaderUniformFloat(int location, float value)
    {
        glUniform1f(location, value);
    }

    void GLRendererAPI::SetShaderUniformVec2(int location, glm::vec2 value)
    {
        glUniform2fv(location, 1, &value[0]);
    }

    void GLRendererAPI::SetShaderUniformVec3(int location, glm::vec3 value)
    {
        glUniform3fv(location, 1, &value[0]);
    }

    void GLRendererAPI::SetShaderUniformVec4(int location, glm::vec4 value)
    {
        glUniform4fv(location, 1, &value[0]);
    }

    void GLRendererAPI::SetShaderUniformMat2(int location, glm::mat2 value)
    {
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GLRendererAPI::SetShaderUniformMat3(int location, glm::mat3 value)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GLRendererAPI::SetShaderUniformMat4(int location, glm::mat4 value)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    
    void GLRendererAPI::DestroyShader(unsigned int shaderID)
    {
        glDeleteShader(shaderID);
    }

    int GLRendererAPI::GetShaderUniformLocation(unsigned int programID, const char* uniformName)
    {
        GLint uniform = glGetUniformLocation(programID, uniformName);
		if(uniform == -1)
			log_error("GLRendererAPI : Could not retrieve uniform %s", uniformName);
		return uniform;
    }

	int GLRendererAPI::GetShaderAttribLocation(unsigned int programID, const char* attribName)
	{
		GLint attrib = glGetAttribLocation(programID, attribName);
		if(attrib == -1)
			log_error("GLRendererAPI : Could not retrieve attrib %s", attribName);
		return attrib;
	}

}
