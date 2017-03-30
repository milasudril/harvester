//@ {
//@	 "targets":[{"name":"harvester.hpp","type":"include"}]
//@	}

#ifndef HARVESTER_HPP
#define HARVESTER_HPP

#include "harvester_impl.hpp"

template<class ExecutionPolicy>
void extract(const char* src_file,const char* dest_dir,ExecutionPolicy&& exec_policy)
	{
	auto eh=[](const char* message,void* policy)
		{reinterpret_cast<ExecutionPolicy*>(policy)->raise(message);}
	auto progress_cb=[](double x,void* policy)
		{reinterpret_cast<ExecutionPolicy*>(policy)->progress(x);}
	extract(src_file,dest_dir,eh,progress_cb);
	}

#endif