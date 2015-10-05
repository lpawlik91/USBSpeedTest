#pragma once


#include <string>
#include <sstream>
#include <iostream>
class DebugPrinter
{
public:
	DebugPrinter() : _enabled(true)
	{
	}
	DebugPrinter(bool enabled) : _enabled(enabled)
	{
	}
	
	DebugPrinter& operator<<(const char ss[])
	{
		if(_enabled)
			std::cout << ss;
		return *this;
	}
	DebugPrinter& operator<<(const int& d)
	{
		if(_enabled)
			std::cout << d;
		return *this;
	}
	void set(bool enabled)
	{
		_enabled = enabled;
	}

private:
	bool _enabled;
};
