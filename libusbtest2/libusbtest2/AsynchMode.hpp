#pragma once

#include "Mode.hpp"

#include <iostream>
#include <pthread.h>
#include <unistd.h>

class AsynchMode : public Mode
{
public:
	AsynchMode(int bufforSize, int count, int vid, int pid, bool printOnlyResult);
	virtual ~AsynchMode();
	virtual int doTest() override;
	virtual void initProcedures() override;

	void closeLibUsb() override;
private:
	pthread_mutex_t _sender_lock;
	pthread_mutex_t _receiver_lock;
	pthread_cond_t _sender_cond;
	pthread_cond_t _receiver_cond;

	libusb_transfer* _senderTransfer;
	libusb_transfer* _receiverTransfer;
	pthread_t _receiverThread;

};


