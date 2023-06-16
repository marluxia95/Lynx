#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include "resource_manager.h"
#include "lynx_common.h"

namespace Lynx {

	class LYNXENGINE_API Resource {
	public:
		Resource(std::string path) : m_resID(PushID()), m_resPath(path) { }
		Resource() : m_resID(PushID()), m_resPath("unknown") { }
		~Resource() = default;
		long int GetResourceID() { return m_resID; }
		std::string GetResourcePath() { return m_resPath; }

		static long int GetLastID() { return total_ids; }

		friend class ResourceManager;
	private:
		static long int PushID() { return total_ids++; }
		static long int total_ids;
	protected:
		long int m_resID;
		std::string m_resPath;
	};

}
#endif
