//@ {
//@	 "targets":[{"name":"directory.hpp","type":"include"}]
//@	}

#ifndef HARVESTER_DIRECTORY_HPP
#define HARVESTER_DIRECTORY_HPP

#include "strcpy.hpp"
#include "rmdir.hpp"
#include <utility>
#include <cstdlib>
#include <cstring>

namespace Harvester
	{
	class Directory
		{
		public:
			template<class ExceptionHandler>
			Directory(const char* name,ExceptionHandler&& eh)
				{
				released=0;
				m_name=strcpy(name);
				if(m_name==nullptr)
					{eh("Out of memory");}
				}	

			Directory(Directory&& dir) noexcept:
				m_name(dir.m_name),released(dir.released)
				{
				dir.m_name=nullptr;
				dir.released=1;
				}

			Directory& operator=(Directory&& dir) noexcept
				{
				std::swap(m_name,dir.m_name);
				std::swap(released,dir.released);
				return *this;
				}

			Directory(const Directory&)=delete;
			Directory& operator=(const Directory&)=delete;

			~Directory() noexcept
				{
				if(m_name!=nullptr)
					{
					if(!released)
						{rmdir(m_name);}
					free(m_name);
					}
				}

			const char* name() const noexcept
				{return m_name;}

			void contentRelease() noexcept
				{released=1;}

		private:
			template<class ExecutionPolicy>
			friend Directory extract(const char* src_file,const char* dest_dir,ExecutionPolicy&& exec_policy);

			char* m_name;
			bool released;
		};
	}

#endif
