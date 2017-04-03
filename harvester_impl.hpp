//@ {
//@	 "targets":[{"name":"harvester_impl.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"harvester.o","rel":"implementation"}]
//@	}

#ifndef HARVESTERIMPL_HPP
#define HARVESTERIMPL_HPP

namespace Harvester
	{
	class Directory;

	enum class ProgressStatus:int{SKIP,EXTRACT,STOP};

	typedef void (*ExceptionHandler)(const char* message,void* exec_policy);
	typedef ProgressStatus (*ProgressCallback)(double x,const char* file,void* exec_policy);

	Directory extract_impl(const char* src_file,const char* dest_dir
		,ExceptionHandler eh,ProgressCallback cb,void* exec_policy);
	}

#endif