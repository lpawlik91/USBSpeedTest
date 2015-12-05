#!/bin/bash
rm *.cpp *.hpp makefile
cp ../libusbtest2/*.cpp ../libusbtest2/*.hpp ../libusbtest2/makefile .
rm stdafx.cpp
sed -i -- 's/#include "stdafx.h"//g' *.cpp
sed -i -- 's/#include "stdafx.h"//g' *.hpp
sed -i '1s/^/#include "Mode.hpp"/' Mode.cpp
sed -i '1s/^/#include "SynchMode.hpp"/' SynchMode.cpp
sed -i '1s/^/#include "AsynchMode.hpp"/' AsynchMode.cpp

sed -i -- 's/#pragma once/#pragma once\n#include "Mode.hpp"\n/g' SynchMode.hpp
sed -i -- 's/#pragma once/#pragma once\n#include "Mode.hpp"\n/g' AsynchMode.hpp
