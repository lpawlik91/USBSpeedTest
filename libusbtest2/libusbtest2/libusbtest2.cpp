// libusbtest2.cpp : Defines the entry point for the console application.
//

#include "SynchMode.hpp"
#include "AsynchMode.hpp"
#include "libusb.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <memory>


#define BUFFOR_MAX 64
#define LAND_TIGER_VID 0x1fc9
#define LAND_TIGER_PID 0x2002


int main(int argc, char* argv[])
{
	if(argc < 4) 
	{
		std::cout << "use: " << argv[0] << " [S/A] <bufforSize> <count>" << std::endl;
		std::cout << "First parameter tells if application should be run in Synchronous mode or Asynchronous" << std::endl;
		std::cout << "Note that max buffer of LandTiger is " << BUFFOR_MAX << "Bytes" << std::endl;
		return 0; 
	}

	char selectedMode = std::toupper(argv[1][0]);
	int modeNum = 0;
	if(selectedMode != 'A' && selectedMode != 'S')
	{
		std::cout << "mode set is different than A or S, setting synchronous as defautl." << std::endl;
		selectedMode = 'S';
	}
		
	int bufforSize = atoi(argv[2]);
	if(bufforSize > BUFFOR_MAX) 
	{
		std::cout << "bufforSize is grather than 64B, setting 64 as default" << std::endl;
		bufforSize = BUFFOR_MAX;
	}

	int count = atoi(argv[3]);

	std::cout << "Total size to send/receive: " << bufforSize << " x " << count << " = " << bufforSize * count << " Bytes" << std::endl;
	try 
	{
		std::unique_ptr<Mode> mode;

		if(selectedMode == 'A')
			mode.reset(new AsynchMode(bufforSize, count, LAND_TIGER_VID, LAND_TIGER_PID));
		else 
			mode.reset(new SynchMode(bufforSize, count, LAND_TIGER_VID, LAND_TIGER_PID));
		mode->initProcedures();
		mode->doTest();
		mode->printFinalInformation();
		mode->closeLibUsb();
	} catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
