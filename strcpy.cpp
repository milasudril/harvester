//@ {
//@	 "targets":[{"name":"strcpy.o","type":"object"}]
//@	}

#include "strcpy.hpp"
#include <cstring>
#include <cstdlib>

char* Harvester::strcpy(const char* dirname) noexcept
	{
	auto l=sizeof(char)*( strlen(dirname) + 1 );
	auto ret=static_cast<char*>(malloc(l));
	if(ret==nullptr)
		{return ret;}

	memcpy(ret,dirname,l);
	return ret;
	}

void Harvester::free(char* x) noexcept
	{::free(x);}
