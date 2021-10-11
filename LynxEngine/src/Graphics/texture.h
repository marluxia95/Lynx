#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h> 
#include <stdio.h>

namespace Lynx::Graphics {

	enum TextureType {
		TEXTURE_DEFAULT,
		TEXTURE_CUBEMAP
	};
	
	class TextureBase;
	
	class TextureData {
	public:
		TextureData(TextureBase* baseTex) : linkedTexture(baseTex) {}
		TextureData(const char* path, unsigned char* data, int width, int height, int channels) : path(path), data(data), width(width), height(height),
		channels(channels) {}

		~TextureData();
		
		bool LoadFromFile(const char* path);
		void Free();
		const char* GetPath() { return path; }
		unsigned char* GetData() { return data; }
		int GetWidth() { return width; }
		int GetHeight() { return height; }
		
		TextureBase* linkedTexture;
		friend class TextureBase;
		friend class Texture;
	private:
		const char* path;
		int width, height, channels;
		unsigned char* data;
	};

	class TextureBase {
		public:
			TextureBase(TextureType type);
			TextureBase(TextureType type, const char* path);
			virtual void Generate() = 0;
			virtual void Use() = 0;
			virtual void Load() = 0;

			bool IsValid() { return id != -1; }
			const char* GetPath() { return data->path; }
			unsigned int GetTextureID() { return texture; }
			int GetID() { return id; }
			TextureData* GetData() { return data; }
			TextureType GetTextureType() { return type; }

		protected:
			void loadFromFile(const char* path);

		protected:
			TextureType type;
			unsigned int texture = 0;
			TextureData* data;
			int id;
	};

	class Texture : public TextureBase {
	public:
		Texture() : TextureBase(TEXTURE_DEFAULT) { }
		Texture(const char* path);
		
		void Load() { loadFromFile(data->path); }
		void Load(const char* path) { loadFromFile(path); }
		void Generate();
		void Use();

	};
}

#endif

