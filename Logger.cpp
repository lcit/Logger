/*	=========================================================================
	Author: Leonardo Citraro
	Company: 
	Filename: Logger.cpp
	Last modifed: 10.07.2016 by Leonardo Citraro
	Description:	Thread-safe singleton logger.

	=========================================================================

	=========================================================================
*/
#include <limits>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <mutex>
#include <time.h>
#include <unistd.h>
#include "Logger.hpp"

using namespace std;

const std::string current_date_time_str(const std::string& date_format, const std::string& time_format){
	time_t     now = time(0);
	struct tm  tstruct;
	char       cdate[80];
	char       ctime[80];
	tstruct = *localtime(&now);
	strftime(cdate, sizeof(cdate), date_format.c_str(), &tstruct);
	strftime(ctime, sizeof(ctime), time_format.c_str(), &tstruct);
	std::string res = std::string(cdate) + "/" + std::string(ctime);
	return res;
}

void LOG__helper(const std::string& filename, const std::string& type, const std::string& msg){	
	try{
		if(type == Logger::kLogLevelError)
			Logger::instance(filename)->log("[" + current_date_time_str("%Y-%m-%d", "%X") + "]" + Logger::kLogLevelError + msg);
		else if(type == Logger::kLogLevelInfo)
			Logger::instance(filename)->log("[" + current_date_time_str("%Y-%m-%d", "%X") + "]" + Logger::kLogLevelInfo + msg);
		else
			Logger::instance(filename)->log("[" + current_date_time_str("%Y-%m-%d", "%X") + "]" + msg);
	}catch(...){
		// An error occurred somewhere. 
		// The first thing we can do is wait a while and try again
		usleep(100000);
		try{
			if(type == Logger::kLogLevelError)
				Logger::instance(filename)->log("[" + current_date_time_str("%Y-%m-%d", "%X") + "]" + Logger::kLogLevelError + msg);
			else if(type == Logger::kLogLevelInfo)
				Logger::instance(filename)->log("[" + current_date_time_str("%Y-%m-%d", "%X") + "]" + Logger::kLogLevelInfo + msg);
			else
				Logger::instance(filename)->log("[" + current_date_time_str("%Y-%m-%d", "%X") + "]" + msg);
		}catch(...){ 
			// The second attempt failed also.
			// We are obliged to drop the log message.
			return;
		}        
	}
}

const string Logger::kLogLevelInfo  = "[INFO]:\t";
const string Logger::kLogLevelError = "[ERROR]:\t";

std::vector<Logger*> Logger::vpInstance;

std::vector<std::string> Logger::vLogNames;

mutex Logger::sMutex;

Logger* Logger::instance(const std::string& filename){
	static Cleanup cleanup;

	lock_guard<mutex> guard(sMutex);
	// searching if filename exists already
	auto it = std::find(Logger::vLogNames.begin(), Logger::vLogNames.end(), filename);
	if(it == Logger::vLogNames.end()){
		try{
			// vLogNames doesn't contains this filename
			// thus we have to create a new log file
			Logger::vpInstance.push_back(new Logger(filename));
		}catch(...){
			throw;
		}
		Logger::vLogNames.push_back(filename);
		return Logger::vpInstance[Logger::vpInstance.size()-1];
	}else{ 
		// the log file exists already.
		// we only have to find it and return it
		auto index = std::distance(Logger::vLogNames.begin(), it);
		return Logger::vpInstance[index];
	}
	return nullptr;
}

Logger::Cleanup::~Cleanup(){
	lock_guard<mutex> guard(Logger::sMutex);
	for(auto& element : Logger::vpInstance){
		delete element;
		element = nullptr;
	}
	Logger::vpInstance.clear();
	Logger::vLogNames.clear();
}

Logger::~Logger(){}

Logger::Logger(const std::string& filename) : _filename(filename){}

void Logger::log(const std::string& msg){
	lock_guard<mutex> guard(sMutex);
	// open file in write mode + append
	std::fstream file;
	try{
		file.open(_filename, std::ios_base::out | std::ios_base::app);
	}catch(...){
		// In my opinion a Logger should not throw, otherwise we may
		// get stuck in a infinite loop? throw->log->throw->log....
		return;
	}
	// append log
	file << msg << std::endl;    
}




