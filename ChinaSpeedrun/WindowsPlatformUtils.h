#pragma once

#include <string>

namespace cs
{
	class wutil
	{
	public:
		static std::string OpenFile(const char* filter = "../resources");
		static std::string SaveFile(const char* filter = "../resources");
	};
}

