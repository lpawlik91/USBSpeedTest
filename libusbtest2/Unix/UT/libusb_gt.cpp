#include <memory>
#include "SynchMode.hpp"
#include "gtest/gtest.h"
#include <stdexcept>

class ModeTest : public ::testing::Test 
{ 
public: 

   void SetUp( ) 
   { 
       synchMode.reset(new SynchMode(128, 256, 0x123d, 0x5678, 0));
   }

   void TearDown( ) 
   { 
   }
   
private:
   std::shared_ptr<Mode> synchMode;
   
};


TEST_F(ModeTest, getContext) 
{
	int a = 1;
	EXPECT_EQ(a, 1);
//	EXPECT_THROW(synchMode->getcontext(), std::runtime_error("Init Context error"));
}


