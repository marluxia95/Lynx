#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <stdio.h>
#include "lynx_common.h"
#include "Core/resource.h"

namespace Lynx::Graphics {

	enum TextureType {
		TEXTURE_DEFAULT,
		TEXTURE_CUBEMAP
	};
	
	class TextureBase;

	class TextureBase : public ResourceBase {
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

			static std::shared_ptr<TextureBase> CreateTexture(std::string path);

		private:
			static int total_textures;

		protected:
			TextureType type;
			unsigned int texture = 0;
			int id;
	};
}

#endif

