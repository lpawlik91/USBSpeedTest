#pragma once

#include "Mode.hpp"

class SynchMode : public Mode
{
public:
	SynchMode(int bufforSize, unsigned count, int vid, int pid, bool printOnlyResult);
	virtual ~SynchMode();
	virtual int doTest() override;

private:

};

