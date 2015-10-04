// libusbtest2.cpp : Defines the entry point for the console application.
//

#include "SynchMode.hpp"
#include "libusb.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <stdexcept>


#define BUFFOR_MAX 64
#define LAND_TIGER_VID 0x1fc9
#define LAND_TIGER_PID 0x2002


int main(int argc, char* argv[])
{
	if(argc < 3) 
	{
		std::cout << "use: libusbtest <bufforSize> <count>" << std::endl;
		std::cout << "Note that max buffer of LandTiger is " << BUFFOR_MAX << "Bytes" << std::endl;
		return 0; 
	}
	
	int bufforSize = atoi(argv[1]);
	if(bufforSize > BUFFOR_MAX) 
	{
		std::cout << "bufforSize is grather than 64B, setting 64 as default" << std::endl;
		bufforSize = BUFFOR_MAX;
	}

	int count = atoi(argv[2]);

	std::cout << "Total size to send/receive: " << bufforSize << " x " << count << " = " << bufforSize * count << " Bytes" << std::endl;
	try 
	{
		SynchMode synchMode(bufforSize, count, LAND_TIGER_VID, LAND_TIGER_PID);
		synchMode.initProcedures();
		synchMode.doTest();
		synchMode.printFinalInformation();
		synchMode.closeLibUsb();
	} catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
