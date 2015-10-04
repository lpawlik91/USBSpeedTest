#pragma once

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include "libusb.h"


class Mode
{
public:
	Mode(int bufforSize, int count, int vid, int pid) : _bufforSize(bufforSize), _count(count), _vid(vid), _pid(pid)
	{
		//_ctx = nullptr;
		//_dev_handle = nullptr;
//		initProcedures();
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
	int _count;
	int _vid;
	int _pid;
	double _timeResult;
	libusb_context* _ctx;
	libusb_device_handle* _dev_handle;

};

