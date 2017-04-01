//@ {
//@	 "targets":[{"name":"harvester.hpp","type":"include"}]
//@	}

#ifndef HARVESTER_HPP
#define HARVESTER_HPP

#include "directory.hpp"
#include "harvester_impl.hpp"

namespace Harvester
	{
	template<class ExecutionPolicy>
	Directory extract(const char* src_file,const char* dest_dir,ExecutionPolicy&& exec_policy)
		{
		auto eh=[](const char* message,void* policy)
			{reinterpret_cast<ExecutionPolicy*>(policy)->raise(message);};
		auto progress_cb=[](double x,const char* file,void* policy)
			{reinterpret_cast<ExecutionPolicy*>(policy)->progress(x,file);};
		return extract(src_file,dest_dir,eh,progress_cb,&exec_policy);
		}
	}

#endif