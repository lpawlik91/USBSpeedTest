// libusbtest2.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "libusb.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

#define BUFFOR_MAX 64
#define LAND_TIGER_VID 0x1fc9
#define LAND_TIGER_PID 0x2002


pthread_mutex_t sender_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t receiver_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sender_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t receiver_cond = PTHREAD_COND_INITIALIZER;

struct TransferStatus
{
	time_t startTest;
	time_t stopTest;
	int allCompleted;
	libusb_transfer* senderHandler;
	libusb_transfer* receiverHandler;
	int sendCount;
	int receivedCount;
	int needToBeSendReceived;
	libusb_context* ctx;
	int waitForSender;
	int waitForReceiver;
	int particularSendComplete;
	int particularReceiveComplete;
};


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

static void LIBUSB_CALL cb_send(struct libusb_transfer *transfer)
{
	if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		fprintf(stderr, "\ndupadupa\n\n");
		//request_exit(2);
	}
	TransferStatus* ts = static_cast<TransferStatus*>(transfer->user_data);
	ts->sendCount++;
	pthread_mutex_lock(&receiver_lock);
	ts->waitForReceiver = 0;
	ts->particularSendComplete = 1;
	pthread_cond_signal(&receiver_cond);
	pthread_mutex_unlock(&receiver_lock);

}

static void LIBUSB_CALL cb_read(struct libusb_transfer *transfer)
{
	if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		fprintf(stderr, "\ndupadupa\n\n");
		//request_exit(2);
		return ;
	}
	TransferStatus* ts = static_cast<TransferStatus*>(transfer->user_data);
	ts->receivedCount++;
	ts->particularReceiveComplete = 1;
	
	pthread_mutex_lock(&sender_lock);
	ts->waitForSender = 0;
	pthread_cond_signal(&sender_cond);
	pthread_mutex_unlock(&sender_lock);
	if(ts->receivedCount == ts->needToBeSendReceived)
	{
		time(&ts->stopTest);
		ts->allCompleted = 1;
	}

}
void* handlerThread(void* arg)
{
	TransferStatus* transferStatus = static_cast<TransferStatus*>(arg);
	while(transferStatus->allCompleted != 1) 
	{
		pthread_mutex_lock(&receiver_lock);
		while(transferStatus->waitForReceiver) {
			pthread_cond_wait(&receiver_cond, &receiver_lock);
		}
		transferStatus->waitForReceiver = 1;

		pthread_mutex_unlock(&receiver_lock);
		transferStatus->particularReceiveComplete = 0;
		libusb_submit_transfer(transferStatus->receiverHandler);
		while(transferStatus->particularReceiveComplete == 0)
			libusb_handle_events(transferStatus->ctx);
	}
}

int doTest(libusb_context* ctx, libusb_device_handle* dev_handle, int bufforSize, int count, double* timeResult, libusb_transfer* transfer, libusb_transfer* listener, pthread_t* listenerThread)
{
	unsigned char *data_out = new unsigned char[bufforSize]; //data to write
	unsigned char *data_in = new unsigned char[bufforSize]; //data to read
	generateSymulatedData(data_out, bufforSize);
	int howManyBytesIsSend; 
	int howManyBytesReceived;
	TransferStatus transferStatus;
	transferStatus.allCompleted = 0;
	
	transferStatus.sendCount = 0;
	transferStatus.receivedCount = 0;
	transferStatus.senderHandler = transfer;
	transferStatus.receiverHandler = listener;
	transferStatus.needToBeSendReceived = count;
	transferStatus.ctx = ctx;
	transferStatus.waitForSender = 0;
	transferStatus.waitForReceiver = 1;
	libusb_fill_bulk_transfer(transfer, dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data_out, bufforSize, cb_send, &transferStatus, 0);
	libusb_fill_bulk_transfer(listener, dev_handle, (2 | LIBUSB_ENDPOINT_IN), data_in, bufforSize, cb_read, &transferStatus, 0);

	*timeResult = 0.0;
	if(pthread_create(listenerThread, NULL, handlerThread, &transferStatus) != 0)
	{
		std::cerr << "Error creating listener thread." << std::endl;
		return 1;
	}
	
	time(&transferStatus.startTest); 
	for(int i = 0; i < count; ++i)
	{
		pthread_mutex_lock(&sender_lock);
		while(transferStatus.waitForSender){
		//	std::cout << "SENDING: in loop" << std::endl;
			pthread_cond_wait(&sender_cond, &sender_lock);
		}
		transferStatus.waitForSender = 1;

		pthread_mutex_unlock(&sender_lock);
		transferStatus.particularSendComplete = 0;
		libusb_submit_transfer(transfer);
		while(transferStatus.particularSendComplete == 0)
			libusb_handle_events(ctx);
		
	}
	pthread_join(*listenerThread, NULL);
	*timeResult = difftime(transferStatus.stopTest, transferStatus.startTest); 
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
		return 0; 
	}
	
	int bufforSize = atoi(argv[1]);
	if(bufforSize > BUFFOR_MAX) 
	{
		std::cout << "bufforSize is grather than 64B, setting 64 as default" << std::endl;
		bufforSize = BUFFOR_MAX;
	}

	int count = atoi(argv[2]);

	if(pthread_mutex_init(&sender_lock, NULL) != 0 || pthread_mutex_init(&receiver_lock, NULL) != 0)
	{
		std::cerr << "Mutex Init Failed!" << std::endl;
		return 1;
	}

	std::cout << "Total size to send/receive: " << bufforSize << " x " << count << " = " << bufforSize * count << " Bytes" << std::endl;

	libusb_context *ctx = getContext(); 
	libusb_device_handle* dev_handle = getDeviceHandle(ctx);
	libusb_transfer* transfer1 = libusb_alloc_transfer(0);
	libusb_transfer* respForTransfer1 = libusb_alloc_transfer(0);
	pthread_t eventHandlerThread;

	libusb_transfer* listener = libusb_alloc_transfer(0);
		if(ctx == NULL || dev_handle == NULL || transfer1 == NULL)
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

	int testStatus = doTest(ctx, dev_handle, bufforSize, count, &testResult, transfer1, listener, &eventHandlerThread);
	pthread_mutex_destroy(&sender_lock);
	pthread_mutex_destroy(&receiver_lock);
	if(testStatus != 0) 
	{
		std::cout << "There was an error during tests!!" << std::endl;
	}
	else
	{
		std::cout << "Sending of: " << bufforSize * count << "Bytes using bufferSize=" << bufforSize << " takes " << testResult << "s." << std::endl;

	}
	libusb_free_transfer(listener);
	libusb_free_transfer(transfer1);
	if(closeLibUsb(dev_handle, ctx) != 0)
	{
		return 1;
	}

	return 0;
}
