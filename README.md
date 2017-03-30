Harvester -- Harvester ARchiVe ExSTroR
======================================
This library exposes one function to extract a file archive

	template<class ExecutionPolicy>
    void extract(const char* src,const char* dest_dir,ExecutionPolicy&& exec_policy);

An `ExecutionPolicy` must have two members

  * `raise(const char* message)`, that is called up on does not return.
  * `progress(double x)`, that is called regulary during the extraction process.

