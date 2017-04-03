//@	{"targets":[{"name":"cherrypick_test","type":"application"}]}

#include "harvester.hpp"
#include <cstdio>

struct ExecutionPolicy
	{
	void raise(const char* message)
		{
		std::array<char,512> msg;
	
		strncpy(msg.data(),message,511);
		msg[511]='\0';
		throw msg;
		}

	Harvester::ProgressStatus progress(double x,const char* file) noexcept
		{
		printf("[%d%%] Extracting %s\n",static_cast<int>(100*x),file);
		return Harvester::ProgressStatus::EXTRACT;
		}
	};

int main()
	{
	Harvester::extract("foo.tar.gx"
		,"/tmp"
		,ExecutionPolicy{}
		,"foo","bar");
	}
