#include <memory>
#include "SynchMode.hpp"
#include "AsynchMode.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <stdexcept>

class SynchTest : public ::testing::Test 
{ 
public: 

   void SetUp( ) 
   { 
       synchMode.reset(new SynchMode(128, 256, 0x123d, 0x5678, 0));
   }

   void TearDown( ) 
   { 
   }
   
   std::shared_ptr<Mode> synchMode;
   
};

class AsynchTest : public ::testing::Test 
{ 
public: 

   void SetUp( ) 
   { 
       asynchMode.reset(new AsynchMode(128, 256, 0x123d, 0x5678, 0));
   }

   void TearDown( ) 
   { 
   }
   
   std::shared_ptr<Mode> asynchMode;
   
};


TEST_F(SynchTest, generateSymulatedData) 
{
	int testedSize = 100;
	unsigned char testTab[testedSize];
	EXPECT_EQ(synchMode->generateSymulatedData(testTab, testedSize), 0);
	for(int i = 0, letterIterator = 0; i < testedSize - 1; ++i, letterIterator++)
	{
		if ('a' + letterIterator > 'z') letterIterator = 0;
		EXPECT_EQ( *(testTab + i), 'a' + letterIterator);
	}
}

TEST_F(SynchTest, getContext) 
{
	EXPECT_NO_THROW(synchMode->getContext());
}

TEST_F(SynchTest, getDeviceHandle) 
{
	EXPECT_THROW(synchMode->getDeviceHandle(), std::runtime_error);
}

TEST_F(AsynchTest, initProcedures) 
{
	EXPECT_THROW(asynchMode->initProcedures(), std::runtime_error);
}


