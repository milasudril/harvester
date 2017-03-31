//@ {
//@	 "targets":[{"name":"strcpy.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"strcpy.o","rel":"implementation"}]
//@	}

#ifndef HARVESTER_STRCPY_HPP
#define HARVESTER_STRCPY_HPP

namespace Harvester
	{
	char* strcpy(const char* dirname) noexcept;
	void free(char* dirname) noexcept;
	}

#endif
