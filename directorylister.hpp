//@	{
//@	 "targets":[{"name":"directorylister.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"directorylister.o","rel":"implementation"}]
//@	}

#ifndef HARVESTER_DIRECTORYLISTER_HPP
#define HARVESTER_DIRECTORYLISTER_HPP

#include <utility>

namespace Harvester
    {
    class DirectoryLister
        {
        public:
			DirectoryLister(const DirectoryLister&)=delete;
			DirectoryLister& operator=(const DirectoryLister&)=delete;

			DirectoryLister(DirectoryLister&& dir) noexcept:m_impl(dir.m_impl)
				{dir.m_impl=nullptr;}

			DirectoryLister& operator=(DirectoryLister&& dir) noexcept
				{
				std::swap(dir.m_impl,m_impl);
				return *this;
				}


            explicit DirectoryLister(const char* dirname);
            ~DirectoryLister() noexcept;
            const char* read();

        private:
            struct Impl;
            Impl* m_impl;
        };
    }

#endif
