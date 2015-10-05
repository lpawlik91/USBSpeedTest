#include "SynchMode.hpp"

SynchMode::SynchMode(int bufforSize, int count, int vid, int pid, bool printOnlyResult) : Mode(bufforSize, count, vid, pid, printOnlyResult)
{
	//std::cout << "SynchMode::SynchMode" << std::endl;
}
SynchMode::~SynchMode()
{

}

int SynchMode::doTest()
{
	unsigned char *data_out = new unsigned char[_bufforSize]; //data to write
	unsigned char* data_in = new unsigned char[_bufforSize];
	generateSymulatedData(data_out, _bufforSize);
	int howManyBytesIsSend; 
	int howManyBytesReceived;

	time_t start_t, end_t;
    	_timeResult = 0;

	
	time(&start_t);
	for(int i = 0; i < _count; ++i)
	{
		int sendStatus = libusb_bulk_transfer(_dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data_out, _bufforSize, &howManyBytesIsSend, 0); 
		if(sendStatus == 0 && howManyBytesIsSend == _bufforSize)
		{
			//here was printing data for debugging only
		}
		else
		{
			//std::cout<< "Write Error" << std::endl;
			delete [] data_out;
			throw std::runtime_error("Write Error!");
			//return 1;
		}
		
		
		int readStatus = libusb_bulk_transfer(_dev_handle, (2 | LIBUSB_ENDPOINT_IN), data_in, _bufforSize * sizeof(unsigned char), &howManyBytesReceived, 0);
		if (readStatus == 0 && howManyBytesReceived == howManyBytesIsSend) 
		{
			//here was printing data for debugging only
		} 
		else 
		{
			//std::cout << "Read Error: " << readStatus << std::endl;
			delete[] data_in;
			throw std::runtime_error("Read Error! ");
			//return 1;
		}
		
	}
	time(&end_t);
	_timeResult = difftime(end_t, start_t);
	delete[] data_in;
	delete [] data_out;
	return 0;
}
