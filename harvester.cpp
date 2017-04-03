//@ {
//@	 "targets":
//@		[{
//@		 "name":"harvester.o","type":"object","pkgconfig_libs":["libarchive","uuid"]
//@		}]
//@	}

#include "harvester_impl.hpp"
#include "directory.hpp"
#include "fileinfo.hpp"
#include <archive.h>
#include <archive_entry.h>
#include <uuid.h>
#include <memory>
#include <array>

using namespace Harvester;

namespace
	{
	struct ArchiveDeleter
		{
		void operator()(archive* obj) noexcept
			{archive_read_free(obj);}
		};

	typedef std::unique_ptr<archive,ArchiveDeleter> ArchiveHandle;


	struct EntryDeleter
		{
		void operator()(archive_entry* obj) noexcept
			{archive_entry_free(obj);}
		};

	typedef std::unique_ptr<archive_entry,EntryDeleter> EntryHandle;

	}

static std::array<char,37> uuid_generate() noexcept
	{
	std::array<char,37> ret;
	uuid_t id;
	::uuid_generate(id);
	uuid_unparse_upper(id,ret.begin());
	return ret;
	}

static bool absolute(const char* str) noexcept
	{
	auto ch=*str;
	if(ch=='/')
		{return 1;}
	if(ch=='\\')
		{return 1;}
	
	if(strlen(str)>2)
		{
		if( ( (ch>='A' && ch<='Z') || (ch>='a' && ch<='z') ) 
			&& str[1]==':'
			&& str[2]=='\\' )
			{return 1;}
		}
	return 0;
	}

static int data_copy(archive* ar,archive* aw) noexcept
	{
	const void* buff;
	size_t size;
	int64_t offset;
	while(1)
		{
		auto status=archive_read_data_block(ar,&buff,&size,&offset);
		if(status==ARCHIVE_EOF)
			{return ARCHIVE_OK;}

		if(status!=ARCHIVE_OK)
			{return status;}
		
		status=archive_write_data_block(aw,buff,size,offset);
		if(status!=ARCHIVE_OK)
			{return status;}
		}
	}

Directory Harvester::extract_impl(const char* src_file,const char* dest_dir
	,ExceptionHandler eh,ProgressCallback cb,void* exec_policy)
	{
	ArchiveHandle a(archive_read_new());
	auto handle=a.get();
	if(handle==nullptr)
		{eh("Libarchive init error",exec_policy);}

	archive_read_support_filter_all(handle);
	archive_read_support_format_all(handle);

	auto status=archive_read_open_filename(handle,src_file,16384);

	if(status!=ARCHIVE_OK)
		{eh(archive_error_string(handle),exec_policy);}

	auto dest_dir_full=std::string(dest_dir);
	dest_dir_full+='/';
	dest_dir_full+=uuid_generate().data();
	mkdir(dest_dir_full.c_str(),S_IRWXU);

	Directory ret
		{
		 dest_dir_full.data()
		,[exec_policy,eh](const char* x){eh(x,exec_policy);}
		};

	EntryHandle e(archive_entry_new());
	auto entry=e.get();
	if(entry==nullptr)
		{eh("Failed to allocate an archive entry object",exec_policy);}
	auto input_size=static_cast<double>( FileInfo(src_file).size() );

	ArchiveHandle a1(archive_write_disk_new());
	auto handle_out=a1.get();
	if(handle_out==nullptr)
		{eh("Failed to create output stream",exec_policy);}
	archive_write_disk_set_options(handle_out
		,ARCHIVE_EXTRACT_TIME|ARCHIVE_EXTRACT_NO_OVERWRITE
		|ARCHIVE_EXTRACT_SECURE_SYMLINKS
		|ARCHIVE_EXTRACT_SECURE_NODOTDOT
		);
	archive_write_disk_set_standard_lookup(handle_out);
	while(1)
		{
		status=archive_read_next_header2(handle,entry);
		if(status==ARCHIVE_EOF)
			{return std::move(ret);}

		if(status!=ARCHIVE_OK)
			{eh(archive_error_string(handle),exec_policy);}

		auto path=archive_entry_pathname(entry);

		if(absolute(path))
			{eh("The given archive contains an absolute path",exec_policy);}

		auto pos=static_cast<uint64_t>( archive_filter_bytes(handle,-1) );
		switch( cb(pos/input_size,path,exec_policy) )
			{
			case ProgressStatus::SKIP:
				break;
			case ProgressStatus::EXTRACT:
				{
				std::string path_out(dest_dir_full);
				path_out+='/';
				path_out+=path;
				archive_entry_set_pathname(entry,path_out.c_str());
				status=archive_write_header(handle_out,entry);
				if(status!=ARCHIVE_OK)
					{eh("Archive contains an invalid entry",exec_policy);}
				if(archive_entry_size(entry) > 0)
					{
					status=data_copy(handle,handle_out);
					if(status!=ARCHIVE_OK)
						{eh(archive_error_string(handle),exec_policy);}
					}
				}
				break;
			case ProgressStatus::STOP:
				return std::move(ret);
			}
		}
	}
