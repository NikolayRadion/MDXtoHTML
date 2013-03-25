#ifndef __MDXtoHTML_UTILS__
#define __MDXtoHTML_UTILS__

#include <fstream>
#include <string>

using namespace std;

class Logger
{
private: 
    std::ofstream  _log_file;
 
public:
 
    std::ofstream& log_file(void)
	{
		return _log_file;
	} 
 
    void message(const string &message)
	{
		_log_file<<"Message: "<<message<<endl;
	}
    
    void warning(const string &message)
	{
		_log_file<<"Warning: "<<message<<endl;
	}
    
    void  error(const string &message)
	{
		_log_file<<"Warning: "<<message<<endl;
	}
 
	Logger( const string &log_file_name ):_log_file(log_file_name, ios::ate)
	{
		_log_file<<"---"<<endl;
	}
    
    ~Logger(void)
	{
		_log_file.close();
	}
};

extern Logger MDXtoHTML_Log;

#ifdef __DBGOUTPUT__
	//mess, warn, err - текст сообщения/предупреждения/ошибки, ret - возвращаемый результат
	#define MACRO_MESSAGE(mess) {MDXtoHTML_Log.message(##mess##);}
	#define MACRO_MESSAGE_RET(mess,ret) {MDXtoHTML_Log.message(##mess##);return(##ret##);}
	#define MACRO_MESSAGE_RET_VOID(mess) {MDXtoHTML_Log.message(##mess##);return;}

	#define MACRO_WARNING_RET(warn,ret) {MDXtoHTML_Log.warning(##warn##);return(##ret##);}
	#define MACRO_WARNING_RET_VOID(warn) {MDXtoHTML_Log.warning(##warn##);return;}

	#define MACRO_ERROR_RET(err,ret) {MDXtoHTML_Log.error(##err##);return(##ret##);}
	#define MACRO_ERROR_RET_VOID(err) {MDXtoHTML_Log.error(##err##);return;}
#else
	#define MACRO_MESSAGE(mess) {MDXtoHTML_Log.message(##mess##);}	
	#define MACRO_MESSAGE_RET(mess,ret) {return(##ret##);}
	#define MACRO_MESSAGE_RET_VOID(mess) {return;}

	#define MACRO_WARNING_RET(warn,ret) {return(##ret##);}
	#define MACRO_WARNING_RET_VOID(warn) {return;}	
	
	#define MACRO_ERROR_RET(err,ret)  {return(##ret##);}
	#define MACRO_ERROR_RET_VOID(err) {return;}
#endif
 

#endif