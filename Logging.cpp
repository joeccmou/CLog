#include "Logging.h"
#include <stdio.h>
#include <string>
#include <thread>         
#include <mutex>
#include <time.h> 

using namespace std;


/*g_mutex_file:                                                     */
/*g_mutex_terminal:                                                     */
mutex           g_mutex_file; 
mutex           g_mutex_terminal;



Logging::Logging(LOGPLACE_ENUM_t enum_LogPlace)
{
   
    //set output place
    if( enum_LogPlace >= LOGPLACE_NONE && enum_LogPlace < LOGPLACE_INVALID )
    {
        m_LogPlace = enum_LogPlace; 
    }
    else
    {
        m_LogPlace = LOGPLACE_INVALID;
    }
       
    //initialize file name to the format LOG_YYYYMMDD.txt
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    char buf[32] = {0};
    sprintf(buf,"LOG_%04d%02d%02d.txt",timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday);
    m_filename = string(buf);

}

Logging::~Logging()
{
}

int Logging::SetPlace(LOGPLACE_ENUM_t enum_LogPlace)
{
    
    switch (enum_LogPlace)
    {
    case LOGPLACE_NONE:
    case LOGPLACE_FILE:
    case LOGPLACE_TERMINAL:
        m_LogPlace = enum_LogPlace;
        return 0;
    default:
        printf("Parameter Error - Logging::SetPlace, enum_LogPlace = %d \n", enum_LogPlace);        
        return -1;
    }


}

int Logging::Write(const string& log_data)
{
    int ret = 0;

    if(m_LogPlace==LOGPLACE_FILE)
    {

        ret = WriteToFile(log_data);
    }
    else if(m_LogPlace==LOGPLACE_TERMINAL)
    {
        ret = WriteToTerminal(log_data);
    }
    else if (m_LogPlace== LOGPLACE_NONE)
    {
        /* JUST DO NOTHING */
    }  
    else 
    {
        printf("Parameter Error - Logging::Write, m_LogPlace = %d \n", m_LogPlace);        
        ret = (int)WRITE_ERROR_PLACE_WRONG;
    }
    
    return ret;
}

int Logging::WriteToFile(const string& log_data)
{
    int ret = 0;  

    g_mutex_file.lock();

    FILE* p_file = NULL;
    p_file = fopen(m_filename.c_str(),"a+");
    if(p_file==NULL)
    {
        g_mutex_file.unlock();
        return (int)WRITE_ERROR_FILE_OPEN;
    }

    ret = fputs(log_data.c_str(),p_file);
    if(ret<0)
    {
        fclose(p_file);
        g_mutex_file.unlock();
        return (int)WRITE_ERROR_FILE_WRITE;
    }

    ret  = fflush(p_file);
    if(ret!=0)
    {
        fclose(p_file);
        g_mutex_file.unlock();
        return (int)WRITE_ERROR_FILE_FLUSH;
    }

    fclose(p_file);

    g_mutex_file.unlock();

    return 0;

}

int Logging::WriteToTerminal(const string& log_data)
{
    int ret = 0;

    g_mutex_terminal.lock();

    ret  = printf("%s",log_data.c_str());
    if(ret < 0 || ret != log_data.size())
    {
        g_mutex_terminal.unlock();
        return (int)WRITE_ERROR_PRINT_TO_TERMINAL;
    }

    g_mutex_terminal.unlock();

    return 0;


}



