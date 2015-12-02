// libusbtest2.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "SynchMode.hpp"
#include "AsynchMode.hpp"
#include "DebugPrinter.hpp"
#include "libusb.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <cctype>


#define BUFFOR_MAX 64
#define LAND_TIGER_VID 0x1fc9
#define LAND_TIGER_PID 0x2002


int main(int argc, char* argv[])
{
	
	if(argc < 4) 
	{
		std::cout << "use: " << argv[0] << " [S/A] <bufforSize> <fullDataSize> optional:<printOnlyResult>" << std::endl;
		std::cout << "First parameter tells if application should be run in Synchronous mode or Asynchronous" << std::endl;
		std::cout << "Note that max buffer of LandTiger is " << BUFFOR_MAX << "Bytes" << std::endl;
		return 0; 
	}
	bool printOnlyResult = false;
	if(argc == 5)
		printOnlyResult = argv[4][0] == '0' ? false : true;
	
	DebugPrinter debugPrinter(! printOnlyResult);
	char selectedMode = std::toupper(argv[1][0]);
	if(selectedMode != 'A' && selectedMode != 'S')
	{
		debugPrinter << "mode set is different than A or S, setting synchronous as default.\n";
		selectedMode = 'S';
	}
		
	unsigned bufforSize = atoi(argv[2]);
	if(bufforSize > BUFFOR_MAX) 
	{
		debugPrinter << "bufforSize is grather than 64B, setting 64 as default\n";
		bufforSize = BUFFOR_MAX;
	}
	unsigned fullDataSize = atoi(argv[3]);
	unsigned count = fullDataSize/bufforSize;
	if(fullDataSize % bufforSize != 0)
	{
		debugPrinter << "It is impossible to send " << fullDataSize << "B using " << bufforSize << "B, to simplify application work sending " << (++count) * bufforSize << "B\n";
	}
	

	debugPrinter << "Total size to send/receive: " << bufforSize << " x " << count << " = " << bufforSize * count << " Bytes\n";
	try 
	{
		std::unique_ptr<Mode> mode;

		if(selectedMode == 'A')
			mode.reset(new AsynchMode(bufforSize, count, LAND_TIGER_VID, LAND_TIGER_PID, printOnlyResult));
		else 
			mode.reset(new SynchMode(bufforSize, count, LAND_TIGER_VID, LAND_TIGER_PID, printOnlyResult));
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
