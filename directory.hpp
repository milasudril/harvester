//@ {
//@	 "targets":[{"name":"directory.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"directory.o","rel":"implementation"}]
//@	}

#ifndef HARVESTER_DIRECTORY_HPP
#define HARVESTER_DIRECTORY_HPP

namespace Harvester
	{
	class Directory;
	
	template<class ExecutionPolicy>
	Directory extract(const char* src_file,const char* dest_dir,ExecutionPolicy&& exec_policy);

	class Directory
		{
		public:
			Directory(Directory&& dir) noexcept:
				m_name(dir.m_name),released(dir.m_name)
				{
				dir.m_name=nullptr;
				dir.released=1;
				}

			Directory& operator=(Directory&& dir) noexcept
				{
				std::swap(m_name,dir.m_name);
				std::swap(m_name,dir.m_name);
				}

			Directory(const Directory&)=delete;
			Directory& operator=(const Directory&)=delete;
			~Directory();

			const char* name() const noexcept;

			const char* release() noexcept
				{
				released=1;
				return m_name;
				}

		private:
			explicit Directory(const char* name);

			template<class ExecutionPolicy>
			friend Directory extract(const char* src_file,const char* dest_dir,ExecutionPolicy&& exec_policy);

			char* m_name;
			bool released;
		};
	}

#endif
