#include<gtest/gtest.h>
#include"../Logging.h"
#include<thread>

using namespace std;

/************************************************************************************
 * Test Part1: Unit Test for each function 
 * *********************************************************************************/
TEST(testSetPlace,SetPlaceSuccess){

    Logging log_instance(LOGPLACE_INVALID);
    EXPECT_EQ(log_instance.SetPlace(LOGPLACE_NONE),0);
    EXPECT_EQ(log_instance.SetPlace(LOGPLACE_FILE),0);
    EXPECT_EQ(log_instance.SetPlace(LOGPLACE_TERMINAL),0);
}

TEST(testSetPlace,SetPlaceFail){

    Logging log_instance(LOGPLACE_TERMINAL);    
    EXPECT_EQ(log_instance.SetPlace(LOGPLACE_INVALID),-1);    
    EXPECT_EQ(log_instance.SetPlace((LOGPLACE_ENUM_t)100),-1);
    EXPECT_EQ(log_instance.SetPlace((LOGPLACE_ENUM_t)65535),-1);
    EXPECT_EQ(log_instance.SetPlace((LOGPLACE_ENUM_t)65536),-1);
    EXPECT_EQ(log_instance.SetPlace((LOGPLACE_ENUM_t)32768),-1);    
    
}

TEST(testWrite,WriteSuccess){

    Logging log_instance(LOGPLACE_TERMINAL);
    EXPECT_EQ(log_instance.Write("testWrite - LOGPLACE_TERMINAL1\n"),0);
    EXPECT_EQ(log_instance.Write("testWrite - LOGPLACE_TERMINAL2\n"),0);

    log_instance.SetPlace(LOGPLACE_FILE);
    EXPECT_EQ(log_instance.Write("testWrite - LOGPLACE_FILE1\n"),0);
    EXPECT_EQ(log_instance.Write("testWrite - LOGPLACE_FILE2\n"),0);       

    log_instance.SetPlace(LOGPLACE_NONE);
    EXPECT_EQ(log_instance.Write("testWrite - LOGPLACE_NONE\n"),0);       
 
}

TEST(testWrite,WriteFail){

    Logging log_instance(LOGPLACE_INVALID);
    EXPECT_EQ(log_instance.Write("testWrite - WriteFail1\n"),WRITE_ERROR_PLACE_WRONG);     

    log_instance.SetPlace((LOGPLACE_ENUM_t)100);   
    EXPECT_EQ(log_instance.Write("testWrite - WriteFail2\n"),WRITE_ERROR_PLACE_WRONG);     
 
    log_instance.SetPlace((LOGPLACE_ENUM_t)65536);   
    EXPECT_EQ(log_instance.Write("testWrite - WriteFail3\n"),WRITE_ERROR_PLACE_WRONG);     
}


TEST(testWriteToFile,WriteToFileSuccess){

    Logging log_instance1(LOGPLACE_FILE);    
    Logging log_instance2(LOGPLACE_FILE);    
    EXPECT_EQ(log_instance1.WriteToFile("testWriteToFile - WriteToFileSuccess1\n"),0);
    EXPECT_EQ(log_instance2.WriteToFile("testWriteToFile - WriteToFileSuccess2\n"),0);  
 
}

TEST(testWriteToFile,WriteToFileFail){

    Logging log_instance(LOGPLACE_FILE);
    std::string  filename = log_instance.m_filename;
    log_instance.m_filename = "";
    EXPECT_EQ(log_instance.WriteToFile("testWriteToFile - WRITE_ERROR_FILE_OPEN\n"),WRITE_ERROR_FILE_OPEN);
    log_instance.m_filename = filename;

    /*CANNOT cause other file operating errors here, finish*/

}

TEST(testWriteToTerminal,WriteToTerminalSuccess){

    Logging log_instance1(LOGPLACE_TERMINAL);
    Logging log_instance2(LOGPLACE_TERMINAL);    
    EXPECT_EQ(log_instance1.WriteToTerminal("testWriteToTerminal - WriteToTerminalSuccess1\n"),0);
    EXPECT_EQ(log_instance2.WriteToTerminal("testWriteToTerminal - WriteToTerminalSuccess2\n"),0);  
 
}

TEST(testWriteToTerminal,WriteToTerminalFail){

    /*CANNOT cause errors here, abandon */
 
}

/************************************************************************************
 * Test Part2: Multi-thread Test
 * TO BE DONE 
 * *********************************************************************************/
#if 1


void TestMultiThread(string title, Logging* log)
{
    int   k = 0;
    char  buf[32] = {0};
    std::thread::id thread_id = std::this_thread::get_id();
    for(;k<1000;k++)
    {
        std::stringstream ss;
        ss << thread_id;
        sprintf(buf,"thread_id[%s] - Num[%d]\n", ss.str().c_str(), k);
        log->Write (title + std::string(buf));
    }
    
}

class LoggingTestMultiThreadToFile : public ::testing::Test {
 
 public:
    LoggingTestMultiThreadToFile():
    m_logging1_(LOGPLACE_FILE),
    m_logging2_(LOGPLACE_FILE),
    m_logging3_(LOGPLACE_FILE)
    {

    }

 protected:
  void SetUp() override {
    //  m_logging1_.SetPlace(LOGPLACE_FILE);
    //  m_logging2_.SetPlace(LOGPLACE_FILE);
    //  m_logging3_.SetPlace(LOGPLACE_FILE);
  }

  // void TearDown() override {}
  void LaunchTest()
  {
      string title = "[FILE] ";
      thread thread1(TestMultiThread,title,&m_logging1_);
      thread thread2(TestMultiThread,title,&m_logging2_);
      thread thread3(TestMultiThread,title,&m_logging3_);

      thread1.join();
      thread2.join();
      thread3.join();
  }

  Logging    m_logging1_;
  Logging    m_logging2_;
  Logging    m_logging3_;
};

class LoggingTestMultiThreadToTerminal : public ::testing::Test {
 public:
    LoggingTestMultiThreadToTerminal():
    m_logging1_(LOGPLACE_TERMINAL),
    m_logging2_(LOGPLACE_TERMINAL),
    m_logging3_(LOGPLACE_TERMINAL)
    {

    }
 
 protected:
  void SetUp() override {
    //  m_logging1_.SetPlace(LOGPLACE_TERMINAL);
    //  m_logging2_.SetPlace(LOGPLACE_TERMINAL);
    //  m_logging3_.SetPlace(LOGPLACE_TERMINAL);
  }

  // void TearDown() override {}
  void LaunchTest()
  {
      string title = "[TERMINAL] ";
      thread thread1(TestMultiThread,title,&m_logging1_);
      thread thread2(TestMultiThread,title,&m_logging2_);
      thread thread3(TestMultiThread,title,&m_logging3_);

      thread1.join();
      thread2.join();
      thread3.join();
  }


  Logging    m_logging1_;
  Logging    m_logging2_;
  Logging    m_logging3_;
};

class LoggingTestMultiThreadToRandom : public ::testing::Test {
public:
    LoggingTestMultiThreadToRandom():
    m_logging1_(LOGPLACE_FILE),
    m_logging2_(LOGPLACE_FILE),
    m_logging3_(LOGPLACE_TERMINAL),
    m_logging4_(LOGPLACE_FILE),
    m_logging5_(LOGPLACE_TERMINAL)
    {

    }
 
protected:
    void SetUp() override {
        m_logging1_.SetPlace(LOGPLACE_FILE);
        m_logging2_.SetPlace(LOGPLACE_FILE);
        m_logging3_.SetPlace(LOGPLACE_TERMINAL);
        m_logging4_.SetPlace(LOGPLACE_FILE);
        m_logging5_.SetPlace(LOGPLACE_TERMINAL);
    }

    // void TearDown() override {}
    void LaunchTest()
    {
        string title = "[RANDOM-";
        thread thread1(TestMultiThread,title+"F]",&m_logging1_);
        thread thread2(TestMultiThread,title+"F]",&m_logging2_);
        thread thread3(TestMultiThread,title+"T]",&m_logging3_);
        thread thread4(TestMultiThread,title+"F]",&m_logging4_);
        thread thread5(TestMultiThread,title+"T]",&m_logging5_);

        thread1.join();
        thread2.join();
        thread3.join();
        thread4.join();
        thread5.join();
    }

    Logging    m_logging1_;
    Logging    m_logging2_;
    Logging    m_logging3_;
    Logging    m_logging4_;
    Logging    m_logging5_;
};


TEST_F(LoggingTestMultiThreadToFile,MultiThread){
    
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    EXPECT_EXIT(LaunchTest(),::testing::ExitedWithCode(0),"Success");

}

TEST_F(LoggingTestMultiThreadToTerminal,MultiThread){
    
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    EXPECT_EXIT(LaunchTest(),::testing::ExitedWithCode(0),"Success");

}

TEST_F(LoggingTestMultiThreadToRandom,MultiThread){
    
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    EXPECT_EXIT(LaunchTest(),::testing::ExitedWithCode(0),"Success");

}



#endif

int main(int argc,char **argv)
{
    int ret = 0;
    testing::InitGoogleTest(&argc,argv);

    ret = RUN_ALL_TESTS();
    return ret;

}