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
#include "fileinfo.hpp"
#include <stack>
#include <utility>
#include <string>
#include <cstring>
#include <cstdio>

using namespace Harvester;

void Harvester::rmdir(const char* dirname) noexcept
	{
	try
		{
		std::stack<std::pair<std::string,DirectoryLister>> nodes;
		nodes.push({dirname,DirectoryLister(dirname)});
		while(!nodes.empty())
			{
			auto& x=nodes.top();
			
			const char* entry=x.second.read();
			if(entry==nullptr)
				{
				remove(x.first.c_str());
				nodes.pop();
				}

			while(entry!=nullptr)
				{
				std::string fullpath(x.first);
				fullpath+='/';
				fullpath+=entry;
				FileInfo info(fullpath.c_str());
				if(info.type()==FileInfo::Type::DIRECTORY)
					{
					if(strcmp(entry,"..") && strcmp(entry,"."))
						{
						nodes.push({fullpath,DirectoryLister(fullpath.c_str())});
						}
					}
				else
					{remove(fullpath.c_str());}
				entry=x.second.read();
				}
			}
		}
	catch(...)
		{fprintf(stderr,"Error\n");}
	}
#endif


