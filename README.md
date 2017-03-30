Harvester -- High-level ARchiVe ExSTactoR
=========================================

This library exposes a very high-level interface to `libarchive` for extracting file archives.

Basic interface
---------------

It consists of one class, `Directory`, that manages the lifetime of the extracted files.

	namespace Harvester
		{
		class Directory
			{
			public:
				Directory(Directory&& dir) noexcept;
				Directory& operator=(Directory&& dir) noexcept;
				Directory(const Directory&)=delete;
				Directory& operator=(const Directory&)=delete;
				~Directory();

				const char* name() const noexcept;
				const char* release() noexcept;
			};
		}

The archive content is extracted by the function

	template<class ExecutionPolicy>
    Directory extract(const char* src_file,const char* dest_dir,ExecutionPolicy&& exec_policy);

An `ExecutionPolicy` must have two members

  * `raise(const char* message);`, that is called up on does not return.
  * `progress(double x)`, that is called regulary during the extraction process.

The archive content is extracted to a uniqe directory inside `dest_dir`. The unique directory name is accessible through the `name` method called on the returned `Directory` object. By default, the destructor will remove the created directory. If the directory should be kept, the method `release` needs to be called before the `Directory` object goes out of scope.

The "harvesting" is transactional. That is, if `extract` does not succeed, the all files are removed.


Path validation policy
----------------------
This library is designed to avoid messing up file systems. Therefore, any absolute path inside an archive will generate an exception, and the previously extracted files are removed from disk. An absolute path is any path that

 * Begins with `/` (A UNIX®-style absolute path)
 * Begins with `\` (A UNC path or an absolute path on to the current drive)
 * Begins with `[A-Z]:\\` (A Windows® drive letter)
