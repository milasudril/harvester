//@ {
//@	 "targets":[{"name":"harvester.o","type":"object"}]
//@	}

#include "harvester_impl.hpp"
#include "directory.hpp"

using namespace Harvester;

Directory Harvester::extract(const char* src_file,const char* dest_dir
	,ExceptionHandler eh,ProgressCallback cb,void* exec_policy)
	{
	}