#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <string>
#include <gtest/gtest_prod.h>

/*The enumeration tells where the log data goes*/
typedef enum
{
    LOGPLACE_NONE = 0,
    LOGPLACE_FILE = 1,
    LOGPLACE_TERMINAL = 2,

    LOGPLACE_INVALID

}LOGPLACE_ENUM_t;

typedef enum
{
    WRITE_ERROR_UNKNOWN = -128,
    WRITE_ERROR_PLACE_WRONG ,
    WRITE_ERROR_FILE_OPEN ,
    WRITE_ERROR_FILE_WRITE ,
    WRITE_ERROR_FILE_FLUSH ,
    WRITE_ERROR_PRINT_TO_TERMINAL,      

    WRITE_ERROR_SUCCESS = 0  //indicate the log string has been successfully written to the specific place

}WRITE_ERROR_t;

/*************************************************************************************************************
 * Class Logging: The instance of this class can output strings to local log file or terminal.
 
 * Features: 
 * 1. Multiple instances can output strings to local log file or terminal independently. 
 * 2. All instances share the same log file which has a name format LOG_YYYYMMDD.txt
 * 3. All instances share the same terminal 
*************************************************************************************************************/
class Logging
{
protected:
    LOGPLACE_ENUM_t     m_LogPlace;    
    std::string         m_filename;

public:
    // Logging();
    Logging(LOGPLACE_ENUM_t enum_LogPlace);     //constructor with a parameter which indicates the output place for the log strings
    ~Logging();

    int SetPlace(LOGPLACE_ENUM_t enum_LogPlace);        //use this function to change the output place in run time
    int Write(const std::string& log_data);             //the interface for users to write strings to log
    
protected:
    int WriteToFile(const std::string& log_data);
    int WriteToTerminal(const std::string& log_data);    


    /************* TEST MACRO FOR GOOGLE TEST ***************/
    FRIEND_TEST(testWriteToFile, WriteToFileSuccess);
    FRIEND_TEST(testWriteToFile, WriteToFileFail);
    FRIEND_TEST(testWriteToTerminal, WriteToTerminalSuccess);
    FRIEND_TEST(testWriteToTerminal, WriteToTerminalFail);

};

#endif
