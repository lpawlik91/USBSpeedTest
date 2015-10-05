#pragma once

#include "DebugPrinter.hpp"
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include "libusb.h"


class Mode
{
public:
	Mode(int bufforSize, unsigned count, int vid, int pid, bool printOnlyResult) : _bufforSize(bufforSize), _count(count), _vid(vid), _pid(pid), _printOnlyResult(printOnlyResult)
	{
		_debugPrinter.set(!printOnlyResult);
	}
	virtual ~Mode();
	virtual int generateSymulatedData(unsigned char*, const int);
	virtual void printFinalInformation();
	virtual void initProcedures();
	virtual void getContext();
	virtual void getDeviceHandle();
	virtual void proceedWithInitUsb();
	virtual void closeLibUsb();
	virtual int doTest() = 0;
	
protected:
	int _bufforSize;
	unsigned _count;
	int _vid;
	int _pid;
	double _timeResult;
	DebugPrinter _debugPrinter;
	bool _printOnlyResult;
	libusb_context* _ctx;
	libusb_device_handle* _dev_handle;

};

