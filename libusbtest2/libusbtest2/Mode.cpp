#include "Mode.hpp"

#include "libusb.h"


Mode::~Mode()
{

}
void Mode::printFinalInformation()
{
	_debugPrinter << "Sending of: " << _bufforSize * _count << "Bytes using bufferSize=" << _bufforSize << " takes " << _timeResult << "s.\n";
	if(_printOnlyResult)
	{
		unsigned allSendReceivedData = _bufforSize * _count;
		double sendingTime = _timeResult/2;
		double receivingTime = _timeResult/2;

		printf("%d\t%u\t%u\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", _bufforSize, _count, allSendReceivedData, _timeResult, sendingTime, receivingTime,
							allSendReceivedData/_timeResult, allSendReceivedData/sendingTime, allSendReceivedData/receivingTime);
	}
}
void Mode::initProcedures()
{
	getContext(); 
	getDeviceHandle();
	proceedWithInitUsb();
}

int Mode::generateSymulatedData(unsigned char* data, const int size)
{
	for(int i = 0, letterIterator = 0; i < size; ++i)
	{
		if('a' + letterIterator > 'z') letterIterator = 0;
		*(data + i) = 'a' + letterIterator++ ;
	}
	*(data + size - 1) = '\0';
	return 0;
}
void Mode::getContext()
{
	
	int r = libusb_init(&_ctx);
	if(r < 0) {
		throw std::runtime_error("Init Context error");
		//d::cout<<"Init Context error Error "<< r <<std::endl;
		//turn NULL;
	}

}	
void Mode::getDeviceHandle()
{
	_dev_handle = libusb_open_device_with_vid_pid(_ctx, _vid, _pid);
	if(_dev_handle == NULL)
		throw std::runtime_error("Cannot open device!");
		//std::cout<<"Cannot open device"<<std::endl;
	else
		_debugPrinter << "Device Opened\n";

//	return dev_handle;
}
void Mode::proceedWithInitUsb()
{
	if(libusb_kernel_driver_active(_dev_handle, 0) == 1) { //find out if kernel driver is attached
		_debugPrinter << "Kernel Driver Active\n";
		if(libusb_detach_kernel_driver(_dev_handle, 0) == 0) //detach it
			_debugPrinter << "Kernel Driver Detached!\n";
	}
	int status = libusb_claim_interface(_dev_handle, 1);
	if(status < 0) 
	{
		throw std::runtime_error("Cannot Claim Interface");
		//std::cout<<"Cannot Claim Interface"<<std::endl;
		//return 1;
	}
	_debugPrinter << "Claimed Interface\n";
}
void Mode::closeLibUsb()
{
	int status = libusb_release_interface(_dev_handle, 1); 
	if(status != 0) {
		throw std::runtime_error("Cannot Relase Interface");
//		std::cout<<"Cannot Release Interface"<<std::endl;
//		return 1;
	}
	_debugPrinter << "Released Interface\n";

	libusb_close(_dev_handle);
	libusb_exit(_ctx); 
//	return 0;
}

