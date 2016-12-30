/*  =========================================================================
    Author: Leonardo Citraro
    Company: 
    Filename: Logger.cpp
    Last modifed: 30.12.2016 by Leonardo Citraro
    Description:    Thread-safe singleton logger.

    =========================================================================
    Copyright (c) 2016 Leonardo Citraro <ldo.citraro@gmail.com>

    Permission is hereby granted, free of charge, to any person obtaining a copy of this
    software and associated documentation files (the "Software"), to deal in the Software
    without restriction, including without limitation the rights to use, copy, modify,
    merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
    OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
    =========================================================================
*/
#include "Logger.hpp"
#include <limits>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <mutex>
#include <ctime>
#include <unistd.h>

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

const std::string Logger::kLogLevelInfo  = "[INFO]:\t";
const std::string Logger::kLogLevelError = "[ERROR]:\t";

std::vector<Logger*> Logger::vpInstance;

std::vector<std::string> Logger::vLogNames;

std::mutex Logger::sMutex;

Logger* Logger::instance(const std::string& filename){
    static Cleanup cleanup;

    std::lock_guard<std::mutex> guard(sMutex);
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
    std::lock_guard<std::mutex> guard(Logger::sMutex);
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
    std::lock_guard<std::mutex> guard(sMutex);
    // open file in write mode + append
    std::fstream file;
    try{
        file.open(_filename, std::ios_base::out | std::ios_base::app);
    }catch(...){
        // A logger should never throw?! we might
        // get stuck in a infinite loop!? throw->log->throw->log....
        return;
    }
    // append log
    file << msg << std::endl;
}




