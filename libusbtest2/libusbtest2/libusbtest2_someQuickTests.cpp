// libusbtest2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "libusb.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>



#define MAX_TEMP  131072
//* 1024
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	time_t start;
	
	unsigned char *data = new unsigned char[MAX_TEMP]; //data to write

	for(unsigned dataIt = 0; dataIt < MAX_TEMP; ++dataIt)
	{
		data[dataIt] = 'a'; // ((dataIt % ('A' - 'a')) + 'a') % 255;
	}
	data[MAX_TEMP - 1] = '\0';

	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
	libusb_device_handle *dev_handle; //a device handle
	libusb_context *ctx = NULL; //a libusb session
	int r; //for return values
	ssize_t cnt; //holding number of devices in list
	//cout<<"maxtemp: "<<MAX_TEMP<<endl;
	r = libusb_init(&ctx); //initialize the library for the session we just declared
	if(r < 0) {
		cout<<"Init Error "<<r<<endl; //there was an error
		return 1;
	}
	libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation
			time_t sstart;
	sstart = time(0);
	
	cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
	if(cnt < 0) {
		cout<<"Get Device Error"<<endl; //there was an error
		return 1;
	}

	
	dev_handle = libusb_open_device_with_vid_pid(ctx, 0x1fc9, 0x2002); //0x125f, 0xa15a); //these are vendorID and productID I found for my usb device
	if(dev_handle == NULL)
		cout<<"Cannot open device"<<endl;
	else
		cout<<"Device Opened"<<endl;

	
	libusb_free_device_list(devs, 1); //free the list, unref the devices in it

	
	//data[0]='a';data[1]='b';data[2]='c';data[3]='d';data[4]='\0'; //some dummy values 

	int actual; //used to find out how many bytes were written
	if(libusb_kernel_driver_active(dev_handle, 0) == 1) { //find out if kernel driver is attached
		cout<<"Kernel Driver Active"<<endl;
		if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
			cout<<"Kernel Driver Detached!"<<endl;
	}
	

	
	//cout<<"Data->"<<data<<"<-"<<endl; //just to see the data we want to write : abcd
	cout<<"Writing Data..."<<endl;

	int i = 0;
	//while (i++ < 1024) {
			r = libusb_claim_interface(dev_handle, 1);//0); //claim interface 0 (the first) of device (mine had jsut 1)
	if(r < 0) {
		cout<<"Cannot Claim Interface"<<endl;
		return 1;
	}
	cout<<"Claimed Interface"<<endl;
	for(int testing=0; testing < (1024 * 1); ++testing) {
		//r = libusb_bulk_transfer(dev_handle, (129 | LIBUSB_ENDPOINT_OUT), data, 4, &actual, 0); //my device's out endpoint was 2, found with trial- the device had 2 endpoints: 2 and 129
		r = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data, MAX_TEMP, &actual, 0); //my device's out endpoint was 2, found with trial- the device had 2 endpoints: 2 and 129
		//std::cout  << "save actual: " << actual <<  std::endl;
		//if(r == 0 && actual == MAX_TEMP) //we wrote the 4 bytes successfully
			//cout<<"Writing Successful!"<<endl;
		//else
			//cout<<"Write Error"<<endl;
		if(r != 0 || actual != MAX_TEMP) {
			cout << "Write Error at indx: " << testing << endl;
			break;
		}// else printf("Write OK!\n");

		/*unsigned char* data_r; //[MAX_TEMP];
		data_r = new unsigned char[MAX_TEMP];
		int actual_length;
		//int r2 = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_IN), data_r, sizeof(data), &actual_length, 0);
		int r2 = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_IN), data_r, MAX_TEMP, &actual_length, 0);
		if(r2 != 0 || actual_length != MAX_TEMP) {
			cout << "Read Error at indx: " << testing << endl;
			break;
		} */// else printf("Read OK!\n");
		//std::cout  << "actual_length: " << actual_length << "max_temp " << MAX_TEMP << std::endl;
		//if (r2 == 0 && actual_length == MAX_TEMP) {
			//printf("\n\nRead: %s\n", data_r);

		//} else {
			//printf("\n\nError Read: %c",data_r[0]);
			//error();
		//}
			//delete[] data_r;
	}
	r = libusb_release_interface(dev_handle, 1); //0); //release the claimed interface
	if(r!=0) {
		cout<<"Cannot Release Interface"<<endl;
		return 1;
	}
	cout<<"Released Interface"<<endl;

	//}

	
	libusb_close(dev_handle); //close the device we opened
		
	
	
	libusb_exit(ctx); //needs to be called to end the
		time_t sstop;
	sstop = time(0);
	std::cout << "stop - start" << sstop - sstart << std::endl;
	delete[] data; //delete the allocated memory for data
	int a;
	cin >> a;
	return 0;
}


/*

void printdev(libusb_device *dev) 
{
	libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);
	if (r < 0) {
		cout<<"failed to get device descriptor"<<endl;
		return;
	}

	cout<<"Number of possible configurations: "<<(int)desc.bNumConfigurations<<"  ";
	cout<<"Device Class: "<<(int)desc.bDeviceClass<<"  ";
	//cout<<"VendorID: "<<desc.idVendor<<"  ";
	//cout<<"ProductID: "<<desc.idProduct<<endl;
	printf("VendorID: %x, ProductID: %x \n",desc.idVendor, desc.idProduct);
	libusb_config_descriptor *config;
	int status = libusb_get_config_descriptor(dev, 0, &config);
	
	
	if(status != 0) return;

	 cout<<"Interfaces: "<<(int)(config->bNumInterfaces)<<" ||| ";


	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;
	for(int i=0; i<(int)config->bNumInterfaces; i++) {
		inter = &config->interface[i];
		cout<<"Number of alternate settings: "<<inter->num_altsetting<<" | ";
		for(int j=0; j<inter->num_altsetting; j++) {
			interdesc = &inter->altsetting[j];
			cout<<"Interface Number: "<<(int)interdesc->bInterfaceNumber<<" | ";
			cout<<"Number of endpoints: "<<(int)interdesc->bNumEndpoints<<" | ";
			for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
				epdesc = &interdesc->endpoint[k];
				cout<<"Descriptor Type: "<<(int)epdesc->bDescriptorType<<" | ";
				cout<<"EP Address: "<<(int)epdesc->bEndpointAddress<<" | ";
			}
		}
	}
	cout<<endl<<endl<<endl;
	libusb_free_config_descriptor(config);

}

*/