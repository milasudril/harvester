//@	{
//@	 "targets":[{"name":"fileinfo.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"fileinfo.o","rel":"implementation"}]
//@	}

#ifndef HARVESTER_FILEINFO_HPP
#define HARVESTER_FILEINFO_HPP

#include <cstdint>

namespace Harvester
	{
	class FileInfo
		{
		public:
            enum class Type:uint32_t{FILE,DIRECTORY,OTHER};

			FileInfo()=default;
			FileInfo(const FileInfo&)=default;
			FileInfo& operator=(const FileInfo&)=default;
			FileInfo(FileInfo&&)=default;
			FileInfo& operator=(FileInfo&&)=default;
			~FileInfo()=default;

			explicit FileInfo(const char* filename);

			double timeModified() const noexcept
				{return m_time_modified;}

			double timeAccessed() const noexcept
				{return m_time_accessed;}

			uint64_t size() const noexcept
				{return m_size;}

			Type type() const noexcept
				{return m_type;}

		private:
			double m_time_modified;
			double m_time_accessed;
			uint64_t m_size;
			Type m_type;
		};
	};


#endif
