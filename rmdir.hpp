//@ {
//@	 "targets":[{"name":"rmdir.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"rmdir.o","rel":"implementation"}]
//@	}

#ifndef HARVESTER_RMDIR_HPP
#define HARVESTER_RMDIR_HPP

namespace Harvester
	{
	void rmdir(const char* dirname) noexcept;
	}

#endif
