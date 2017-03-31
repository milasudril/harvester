//@ {
//@	 "targets":[{"name":"rmdir.o","type":"object"}]
//@	}

#include "rmdir.hpp"
#if __has_include(<filesystem>)
#include <filesystem>

namespace fs=std::filesystem;
#elif __has_include(<experimental/filesystem>)

#include <experimental/filesystem>

namespace fs=std::experimental::filesystem;

#else
#error This library requires support C++17 or Filesystem TS
#endif

void Harvester::rmdir(const char* dirname) noexcept
	{
	std::error_code ec;
	fs::remove_all(dirname,ec);
	}
