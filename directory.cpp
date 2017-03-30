//@ {
//@	 "targets":[{"name":"directory.o","type":"object"}]
//@	}

#include "directory.hpp"
#include <cstdlib>
#include <cstring>

using namespace Harvester;

static void recursive_delete(const char* name)
	{}

Directory::~Directory()
	{
	if(m_name!=nullptr)
		{
		if(!released)
			{recursive_delete(m_name);}
		free(m_name);
		}
	}
