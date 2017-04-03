//@ {
//@	 "targets":[{"name":"harvester.hpp","type":"include"}]
//@	}

#ifndef HARVESTER_HPP
#define HARVESTER_HPP

#include "directory.hpp"
#include "harvester_impl.hpp"
#include <array>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <cstddef>

namespace Harvester
	{
	template<class ExecutionPolicy>
	Directory extract(const char* src_file,const char* dest_dir,ExecutionPolicy&& exec_policy)
		{
		auto eh=[](const char* message,void* policy)
			{reinterpret_cast<ExecutionPolicy*>(policy)->raise(message);};
		auto progress_cb=[](double x,const char* file,void* policy)
			{return reinterpret_cast<ExecutionPolicy*>(policy)->progress(x,file);};
		return extract_impl(src_file,dest_dir,eh,progress_cb,&exec_policy);
		}

	template<class ExecutionPolicy>
	Directory extract(const char* src_file,const char* dest_dir
		,ExecutionPolicy& exec_policy
		,const char** files_begin
		,const char** files_end);

	template<class ExecutionPolicy,class... Args>
	Directory extract(const char* src_file,const char* dest_dir
		,ExecutionPolicy&& exec_policy,const char* file,Args... files)
		{
		std::array<const char*,1 + sizeof...(files)> a{file,files...};
		return extract(src_file,dest_dir,exec_policy,a.begin(),a.end());
		}

	template<class ExecutionPolicy>
	class ExecPolicyWrapper
		{
		public:
			ExecPolicyWrapper(const char** files_begin,const char** files_end
				,ExecutionPolicy&& ep):m_files_begin(files_begin)
					,m_files_end(files_end),m_exec_policy(std::move(ep)),hitcount(0)
				{std::sort(m_files_begin,m_files_end,cmp);}

			ProgressStatus progress(double x,const char* file)
				{
				if(hitcount==m_files_end - m_files_begin)
					{return ProgressStatus::STOP;}
				auto i=std::lower_bound(m_files_begin,m_files_end,file,cmp);
				if (i!=m_files_end && !cmp(file,*i))
					{
					++hitcount;
					return m_exec_policy.progress(x,file);
					}
				return ProgressStatus::SKIP;
				}

			void raise(const char* message)
				{m_exec_policy.raise(message);}

		private:
			const char** m_files_begin;
			const char** m_files_end;
			ExecutionPolicy m_exec_policy;
			ptrdiff_t hitcount;
			static bool cmp(const char* a,const char* b)
				{return strcmp(a,b)<0;};
		};

	template<class ExecutionPolicy>
	Directory extract(const char* src_file,const char* dest_dir
		,ExecutionPolicy& exec_policy
		,const char** files_begin
		,const char** files_end)
		{
		ExecPolicyWrapper<ExecutionPolicy> ep(files_begin,files_end,std::move(exec_policy));

		auto eh=[](const char* message,void* policy)
			{reinterpret_cast<ExecPolicyWrapper<ExecutionPolicy>*>(policy)->raise(message);};

		auto progress_cb=[](double x,const char* file,void* policy)
			{return reinterpret_cast<ExecPolicyWrapper<ExecutionPolicy>*>(policy)->progress(x,file);};

		return extract_impl(src_file,dest_dir,eh,progress_cb,&ep);
		}
	}

#endif