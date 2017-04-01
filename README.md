Harvester -- High-level ARchiVe ExSTactoR
=========================================

This library exposes a very high-level interface to `libarchive` for extracting file archives.

Basic interface
---------------

It consists of one class, `Directory`, that manages the lifetime of the extracted files:

	namespace Harvester
		{
		class Directory
			{
			public:
				template<class ExceptionHandler>
				Directory(const char* dir_name,ExceptionHandler&& eh);

				Directory(Directory&& dir) noexcept;
				Directory& operator=(Directory&& dir) noexcept;
				Directory(const Directory&)=delete;
				Directory& operator=(const Directory&)=delete;
				~Directory();

				const char* name() const noexcept;
				void contentRelease() noexcept;
			};
		}

It also contains a function that extracts all files in a directory.

	template<class ExecutionPolicy>
    Directory extract(const char* src_file,const char* dest_dir,ExecutionPolicy&& exec_policy);

An `ExecutionPolicy` must have two members

  * `raise(const char* message)`, that is called up error and does not return. The function *must not* throw `message` directly, since the buffer may be deallocated during stack unwinding. Instead, copy the message into a fixed-size buffer such as `std::array<char,512>` with `strncpy`, and throw that object. 
  * `progress(double x, const char* name)`, that is called regulary during the extraction process.

The archive content is extracted to a uniqe directory inside `dest_dir`, which must exist. The unique directory name is accessible through the `name` method called on the returned `Directory` object. By default, the destructor will remove the created directory. If the directory should be kept, the method `release` needs to be called before the `Directory` object goes out of scope. Notice that `release` *does not* release the directory name from the object. Thus the caller *must not* try to deallocate the name.

The extraction process is transactional. That is, if `extract` does not succeed, all created files are removed.


Path validation policy
----------------------
This library is designed to avoid messing up file systems. Therefore, any absolute path inside an archive will generate an exception, and the previously extracted files are removed from disk. An absolute path is any path that

 * Begins with `/` (A POSIX style absolute path)
 * Begins with `\` (A UNC path or an absolute path on to the current drive)
 * Begins with regex `[A-Za-z]:\\` (A drive letter)

Also paths that contains a `..` element generate an exception.

Example usage
-------------
A simple example can be found in `main.cpp`. It is a fully working program for extracting files from a file archive.
