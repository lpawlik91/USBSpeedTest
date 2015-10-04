#pragma once

#include "Mode.hpp"

class SynchMode : public Mode
{
public:
	SynchMode(int bufforSize, int count, int vid, int pid);
	virtual ~SynchMode();
	virtual int doTest() override;

private:

};

