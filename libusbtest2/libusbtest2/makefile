
libusb: SyncMode.o Mode.hpp Mode.cpp SynchMode.hpp SynchMode.cpp
	g++ -std=c++11 libusbtest2.cpp SynchMode.cpp Mode.cpp -o libusb -I/usr/include/libusb-1.0 -lusb-1.0 -lpthread

SyncMode.o: Mode.o Mode.hpp Mode.cpp SynchMode.hpp SynchMode.cpp
	g++ -std=c++11 -c SynchMode.cpp -o SynchMode.o -I/usr/include/libusb-1.0 -lusb-1.0 -lpthread

Mode.o: Mode.hpp Mode.cpp
	g++ -std=c++11 -c Mode.cpp -o Mode.o -I/usr/include/libusb-1.0 -lusb-1.0 -lpthread


