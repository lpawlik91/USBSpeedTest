// libusbtest2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "libusb.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>


#define BUFFOR_MAX 64
#define LAND_TIGER_VID 0x1fc9
#define LAND_TIGER_PID 0x2002



int generateSymulatedData(unsigned char* data, const int size)
{
	for(int i = 0, letterIterator = 0; i < size; ++i)
	{
		if('a' + letterIterator > 'z') letterIterator = 0;
		*(data + i) = 'a' + letterIterator++ ;
	}
	*(data + size - 1) = '\0';
	return 0;
}

libusb_context* getContext()
{
	libusb_context* ctx = NULL;
	int r = libusb_init(&ctx);
	if(r < 0) {
		std::cout<<"Init Context error Error "<< r <<std::endl;
		return NULL;
	}
	return ctx;
}

libusb_device_handle* getDeviceHandle(libusb_context* ctx)
{
	libusb_device_handle* dev_handle = libusb_open_device_with_vid_pid(ctx, LAND_TIGER_VID, LAND_TIGER_PID);
	if(dev_handle == NULL)
		std::cout<<"Cannot open device"<<std::endl;
	else
		std::cout<<"Device Opened"<<std::endl;

	return dev_handle;

}

int proceedWithInitLibUsb(libusb_device_handle* dev_handle, libusb_context* ctx)
{
		
	if(libusb_kernel_driver_active(dev_handle, 0) == 1) { //find out if kernel driver is attached
		std::cout<<"Kernel Driver Active"<<std::endl;
		if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
			std::cout<<"Kernel Driver Detached!"<<std::endl;
	}
	int status = libusb_claim_interface(dev_handle, 1);
	if(status < 0) 
	{
		std::cout<<"Cannot Claim Interface"<<std::endl;
		return 1;
	}
	std::cout<<"Claimed Interface"<<std::endl;

	return 0;
}

int doTest(libusb_device_handle* dev_handle, int bufforSize, int count, double* timeResult)
{
	unsigned char *data_out = new unsigned char[bufforSize]; //data to write
	unsigned char* data_in = new unsigned char[bufforSize];
	generateSymulatedData(data_out, bufforSize);
	int howManyBytesIsSend; //used to find out how many bytes were written
	int howManyBytesReceived;

	time_t start_t, end_t;
    *timeResult = 0;

	//std::cout<<"Data->"<<data_out<<"<-"<<std::endl; //just to see the data we want to write : abcd
	//std::cout<<"Writing Data..."<<std::endl;
	time(&start_t);
	for(int i = 0; i < count; ++i)
	{
		int sendStatus = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data_out, bufforSize, &howManyBytesIsSend, 0); 
		if(sendStatus == 0 && howManyBytesIsSend == bufforSize)
		{
			//std::cout<<"Writing Successful!"<<std::endl;
		}
		else
		{
			std::cout<< "Write Error" << std::endl;
			delete [] data_out;			
			return 1;
		}
		
		
		int readStatus = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_IN), data_in, bufforSize * sizeof(unsigned char), &howManyBytesReceived, 0);
		if (readStatus == 0 && howManyBytesReceived == howManyBytesIsSend) 
		{
			//printf("\n\nactual_length: %d, Read: %s\n", actual_length, data_r);
		} 
		else 
		{
			std::cout << "Read Error: " << readStatus << std::endl;
			delete[] data_in;
			return 1;
		}
		
	}
	time(&end_t);
	*timeResult = difftime(end_t, start_t);
	delete[] data_in;
	delete [] data_out;
	return 0;
}

int closeLibUsb(libusb_device_handle* dev_handle, libusb_context* ctx)
{
	int status = libusb_release_interface(dev_handle, 1); 
	if(status != 0) {
		std::cout<<"Cannot Release Interface"<<std::endl;
		return 1;
	}
	std::cout<<"Released Interface"<<std::endl;

	libusb_close(dev_handle);
	libusb_exit(ctx); 
	return 0;
}

int main(int argc, char* argv[])
{
	if(argc < 3) 
	{
		std::cout << "use: libusbtest <bufforSize> <count>" << std::endl;
		std::cout << "Note that max buffer of LandTiger is " << BUFFOR_MAX << "Bytes" << std::endl;
		int t;
		std::cin >> t;
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
	
	libusb_context *ctx = getContext(); 
	libusb_device_handle* dev_handle = getDeviceHandle(ctx);
	if(ctx == NULL || dev_handle == NULL)
	{
		return 1;
	}
	int initStatus = proceedWithInitLibUsb(dev_handle, ctx);
	if(initStatus != 0) 
	{
		std::cout << "proceedWithInitLibUsb exited with errors!" << std::endl;
		return 1;
	}
	double testResult = 0.;
	int testStatus = doTest(dev_handle, bufforSize, count, &testResult);
	if(testStatus != 0) 
	{
		std::cout << "There was an error during tests!!" << std::endl;
	}
	else
	{
		std::cout << "Sending of: " << bufforSize * count << "Bytes using bufferSize=" << bufforSize << " takes " << testResult << "s." << std::endl;

	}


	if(closeLibUsb(dev_handle, ctx) != 0)
	{
		return 1;
	}

	return 0;
}
