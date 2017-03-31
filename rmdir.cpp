//@ {
//@	 "targets":[{"name":"rmdir.o","type":"object"}]
//@	}

#include "rmdir.hpp"
#if __has_include(<filesystem>)
#include <filesystem>
void Harvester::rmdir(const char* dirname) noexcept
	{
	std::error_code ec;
	fs::remove_all(dirname,ec);
	}
#else

#include "directorylister.hpp"
#include <stack>
#include <utility>
#include <string>

using namespace Harvester;

void Harvester::rmdir(const char* dirname) noexcept
	{
	try
		{
		std::stack<std::pair<std::string,DirectoryLister>> nodes;
		nodes.push({dirname,DirectoryLister(dirname)});
		while(!nodes.empty())
			{
			auto x=std::move(nodes.top());
			nodes.pop();
			const char* entry;
			while((entry=x.second.read())!=nullptr)
				{
				fprintf(stderr,"%s/%s\n",x.first.c_str(),entry);
				}
			}
		}
	catch(...)
		{}
	}
#endif


