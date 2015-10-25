#include "stdafx.h"
namespace ThreadHelper
{
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
		pthread_mutex_t* sender_lock;
		pthread_mutex_t* receiver_lock;
		pthread_cond_t* sender_cond;
		pthread_cond_t* receiver_cond;
	};


	static void LIBUSB_CALL cb_read(struct libusb_transfer *transfer)
	{
		if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
			std::cerr << "Reading data error!" << std::endl;
			return ;
		}
		TransferStatus* ts = static_cast<TransferStatus*>(transfer->user_data);
		ts->receivedCount++;
		ts->particularReceiveComplete = 1;
	
		pthread_mutex_lock(ts->sender_lock);
		ts->waitForSender = 0;
		pthread_cond_signal(ts->sender_cond);
		pthread_mutex_unlock(ts->sender_lock);
		if(ts->receivedCount == ts->needToBeSendReceived)
		{
			time(&ts->stopTest);
			ts->allCompleted = 1;
		}

	}

	static void LIBUSB_CALL cb_send(struct libusb_transfer *transfer)
	{
		if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
			std::cerr << "Sending data error!" << std::endl;
			return ;
		}
		TransferStatus* ts = static_cast<TransferStatus*>(transfer->user_data);
		ts->sendCount++;
		pthread_mutex_lock(ts->receiver_lock);
		ts->waitForReceiver = 0;
		ts->particularSendComplete = 1;
		pthread_cond_signal(ts->receiver_cond);
		pthread_mutex_unlock(ts->receiver_lock);

	}



	void* receiverThread(void* arg)
	{
		TransferStatus* transferStatus = static_cast<TransferStatus*>(arg);
		while(transferStatus->allCompleted != 1) 
		{
			pthread_mutex_lock(transferStatus->receiver_lock);
			while(transferStatus->waitForReceiver) {
				pthread_cond_wait(transferStatus->receiver_cond, transferStatus->receiver_lock);
			}
			transferStatus->waitForReceiver = 1;

			pthread_mutex_unlock(transferStatus->receiver_lock);
			transferStatus->particularReceiveComplete = 0;
			libusb_submit_transfer(transferStatus->receiverHandler);
			while(transferStatus->particularReceiveComplete == 0)
				libusb_handle_events(transferStatus->ctx);
		}
		return nullptr;
	}

}



AsynchMode::AsynchMode(int bufforSize, unsigned count, int vid, int pid, bool printOnlyResult) : Mode(bufforSize, count, vid, pid, printOnlyResult), 
											_sender_lock(PTHREAD_MUTEX_INITIALIZER), 
											_receiver_lock(PTHREAD_MUTEX_INITIALIZER),
											_sender_cond(PTHREAD_COND_INITIALIZER),
											_receiver_cond(PTHREAD_COND_INITIALIZER)
{
	//std::cout << "AsynchMode::AsynchMode" << std::endl;
}
AsynchMode::~AsynchMode()
{

}

void AsynchMode::initProcedures()
{
	Mode::initProcedures();
	_senderTransfer = libusb_alloc_transfer(0);	
	_receiverTransfer = libusb_alloc_transfer(0);
	if(_senderTransfer == NULL || _receiverTransfer == NULL)
	{
		throw std::runtime_error("Transfer allocation Error");
	}

	if(pthread_mutex_init(&_sender_lock, NULL) != 0 || pthread_mutex_init(&_receiver_lock, NULL) != 0)
	{
		throw std::runtime_error("Mutex Init Failed!");
	}

}

void AsynchMode::closeLibUsb()
{
	pthread_mutex_destroy(&_sender_lock);
	pthread_mutex_destroy(&_receiver_lock);
	libusb_free_transfer(_senderTransfer);
	libusb_free_transfer(_receiverTransfer);
	Mode::closeLibUsb();
}

int AsynchMode::doTest()
{
	unsigned char *data_out = new unsigned char[_bufforSize]; //data to write
	unsigned char *data_in = new unsigned char[_bufforSize]; //data to read
	generateSymulatedData(data_out, _bufforSize);
//	int howManyBytesIsSend; 
//	int howManyBytesReceived;
	ThreadHelper::TransferStatus transferStatus;
	transferStatus.allCompleted = 0;
	
	transferStatus.sendCount = 0;
	transferStatus.receivedCount = 0;
	transferStatus.senderHandler = _senderTransfer;
	transferStatus.receiverHandler = _receiverTransfer;
	transferStatus.needToBeSendReceived = _count;
	transferStatus.ctx = _ctx;
	transferStatus.waitForSender = 0;
	transferStatus.waitForReceiver = 1;
	transferStatus.sender_lock = &_sender_lock;
	transferStatus.sender_cond = &_sender_cond;
	transferStatus.receiver_lock = &_receiver_lock;
	transferStatus.receiver_cond = &_receiver_cond;
	
	libusb_fill_bulk_transfer(_senderTransfer, _dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data_out, _bufforSize, ThreadHelper::cb_send, &transferStatus, 0);
	libusb_fill_bulk_transfer(_receiverTransfer, _dev_handle, (2 | LIBUSB_ENDPOINT_IN), data_in, _bufforSize, ThreadHelper::cb_read, &transferStatus, 0);

	_timeResult = 0.0;
	if(pthread_create(&_receiverThread, NULL, ThreadHelper::receiverThread, &transferStatus) != 0)
	{
		throw std::runtime_error("Error creating listener thread.");
	}
	
	time(&transferStatus.startTest); 
	for(unsigned i = 0; i < _count; ++i)
	{
		pthread_mutex_lock(&_sender_lock);
		while(transferStatus.waitForSender){
		//	std::cout << "SENDING: in loop" << std::endl;
			pthread_cond_wait(&_sender_cond, &_sender_lock);
		}
		transferStatus.waitForSender = 1;

		pthread_mutex_unlock(&_sender_lock);
		transferStatus.particularSendComplete = 0;
		libusb_submit_transfer(_senderTransfer);
		while(transferStatus.particularSendComplete == 0)
			libusb_handle_events(_ctx);
		
	}
	pthread_join(_receiverThread, NULL);
	_timeResult = difftime(transferStatus.stopTest, transferStatus.startTest);
	delete data_out;
	delete data_in;
	return 0;
}
