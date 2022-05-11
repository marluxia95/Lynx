#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <stdio.h>
#include "lynx_common.h"
#include "Core/resource.h"
#include "rendererAPI.h"

namespace Lynx::Graphics {
	
	class TextureBase;

	class TextureBase : public ResourceBase {
		protected:
			typedef struct TextureData {
				unsigned char* data;
				int width, height, channels;
			} TextureData;
		public:
			TextureBase(TextureType type);
			TextureBase(TextureType type, const char* path);
			
			virtual void Generate() = 0;
			virtual void Use() = 0;
			virtual void LoadFromFile(std::string path) = 0;

			bool IsValid() { return id != -1; }
			unsigned int GetTextureID() { return texture; }
			int GetID() { return id; }
			TextureType GetTextureType() { return type; }

			static int PushTextureID() { return total_textures++; }

		private:
			static int total_textures;

		protected:
			TextureType type;
			TextureData data;
			unsigned int texture = 0;
			int id;
	};

	class Texture : public Lynx::Graphics::TextureBase {
    private:
        void loadDDSTex(std::string path);
		void loadSTBTex(std::string path);
    public:
        Texture();
        Texture(std::string path);
        void Generate();
        void Use();
        void LoadFromFile(std::string path);
    };

    class CubemapTexture : public Lynx::Graphics::TextureBase {
    public:
        CubemapTexture();
        CubemapTexture(std::string basepath);
        void Use();
        void LoadFromFile(std::string path);
    };
}

#endif

