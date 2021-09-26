#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h> 
#include <stdio.h>

namespace Lynx::Graphics {

	typedef struct {
		int width, height, channels;
		unsigned char* data;
		const char* path;
	} TextureData;

	enum TextureType {
		TEXTURE_DEFAULT,
		TEXTURE_CUBEMAP
	};

	class TextureBase {
		public:
			TextureBase(TextureType type) : id(-1), type(type) {}
			virtual void Use() = 0;

			bool IsValid() { return id != -1; }
			
			unsigned int GetTextureID() { return texture; }
			int GetID() { return id; }
			TextureType GetTextureType() { return type; }


		protected:
			TextureType type;
			unsigned int texture = 0;
			const char* path;
			int id;
	};

	class Texture : public TextureBase {
	public:
		Texture() : TextureBase(TEXTURE_DEFAULT) { }
		Texture(const char* path);
		
		void Load(const char* path);
		void Generate(int tid);
		void Use();

		
	private:
		TextureData tex_data;
	
	};

	TextureData* loadTexture(const char* path);
	void textureData_free(TextureData* data);

}

#endif

