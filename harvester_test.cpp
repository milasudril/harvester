//@	{"targets":[{"name":"harvester_test","type":"application"}]}

#include "harvester.hpp"

int main()
	{
	Harvester::Directory foo("__targets",[](const char* message){abort();});
	}