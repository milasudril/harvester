//@	{"targets":[{"name":"harvester","type":"application"}]}

#include "harvester.hpp"
#include <array>

struct ExecutionPolicy
	{
	void raise(const char* message)
		{
		std::array<char,512> msg;
	
		strncpy(msg.data(),message,511);
		msg[511]='\0';
		throw msg;
		}

	void progress(double x,const char* file) noexcept
		{
		printf("[%d%%] Extracting %s\n",static_cast<int>(100*x),file);
		}
	};

int main(int argc,char** argv)
	{
	if(argc<3)
		{
		printf("Usage: harvester archive target_directory\n");
		return 0;
		}
	try
		{
		Harvester::extract(argv[1],argv[2],ExecutionPolicy{}).contentRelease();
		}
	catch(const std::array<char,512>& msg)
		{
		printf("%s\n",msg.begin());
		return -1;
		}
	return 0;
	}
