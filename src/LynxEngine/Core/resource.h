#pragma once


namespace Lynx {

    class ResourceBase {
	public:
		ResourceBase(std::string path) : m_resID(PushID()), m_resPath(path) { }
        ResourceBase() : m_resID(PushID()), m_resPath("unknown") { }
        ~ResourceBase() = default;
		long int GetResourceID() { return m_resID; }
		std::string GetResourcePath() { return m_resPath; }

        static long int GetLastID() { return total_ids; }

        friend class ResourceManager;
    private:
        static long int PushID() { total_ids++; }
        static long int total_ids;
	protected:
		long int m_resID;
		std::string m_resPath;
	};

}
