/*  =========================================================================
    Author: Leonardo Citraro
    Company: 
    Filename: Logger.hpp
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
#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <sstream>
#include <vector>
#include <string>
#include <mutex>
#include <ios>
#include <fstream>

/// Utility function to log an error message
///
/// This function can take a temporary string-stream as parameter i.e.: \n
/// LOG_ERROR("example message: i=" << 2); \n
/// The result would be in the form: "[current date and time][ERROR] message"
#define LOG_ERROR(...) GET_MACRO(__VA_ARGS__, LOG__ERROR__spec, LOG__ERROR__main)(__VA_ARGS__)

/// Utility function to log an info message
///
/// This function can take a temporary string-stream as parameter i.e.: \n
/// LOG_INFO("example message: i=" << 2); \n
/// The result would be in the form: "[current date and time][ERROR] message"
#define LOG_INFO(...) GET_MACRO(__VA_ARGS__, LOG__INFO__spec, LOG__INFO__main)(__VA_ARGS__)

/// Utility function to log a custom message
///
/// This function can take a temporary string-stream as parameter i.e.: \n
/// LOG_CUSTOM("example message: i=" << 2); \n
/// The result would be in the form: "[current date and time] message"
#define LOG_CUSTOM(...) GET_MACRO(__VA_ARGS__, LOG__CUSTOM__spec, LOG__CUSTOM__main)(__VA_ARGS__)


// These are helper functions and are not supposed to be used. 
// Use LOG_ERROR, LOG_INFO and LOG_CUSTOM instead!
#define GET_MACRO(_1,_2,NAME,...) NAME
#define LOG__ERROR__main(token){ std::stringstream o; o << token; LOG__helper("log", Logger::kLogLevelError, o.str());}
#define LOG__INFO__main(token){std::stringstream o;o << token;LOG__helper("log", Logger::kLogLevelInfo, o.str());}
#define LOG__CUSTOM__main(token){std::stringstream o;o << token;LOG__helper("log", "", o.str());}
#define LOG__ERROR__spec(filename, token){std::stringstream o;o << token;LOG__helper(filename, Logger::kLogLevelError, o.str());}
#define LOG__INFO__spec(filename, token){std::stringstream o;o << token;LOG__helper(filename, Logger::kLogLevelInfo, o.str());}
#define LOG__CUSTOM__spec(filename, token){std::stringstream o;o << token;LOG__helper(filename, "", o.str());}

/// Utility function to log a message into a specific file
///
/// It cannot take a temporary stringstream as parameter, only strings. 
///
/// @param filename: name of the log file to use
/// @param type: string identifier as kLogLevelInfo or kLogLevelError
/// @param msg: is the actual message
/// @return no return
void LOG__helper(const std::string& filename, const std::string& type, const std::string& msg);

/// Thread-safe singleton logger class. 
///
/// The nice thing about this logger is that you don't need to explicitly create
/// an object nor passing a log object throughout all your project functions in 
/// order to write to the same file! The logger instance is accessible anywhere 
/// in your code by just including the header file #include <Logger.hpp>.
/// You can also create multiple log files! Enjoy.
///
/// Example usage:
/// 
///     Logger::instance("log.txt")->File::append("custom message for log.txt");
///     Logger::instance("log2.txt")->File::append("another custom  message for log2.txt");
///     LOG_CUSTOM("another custom  message for log2.txt");
///     LOG_CUSTOM("log2.txt", "another custom  message for log2.txt");
///     LOG_ERROR("log1.txt", "an error message for log.txt"); 
class Logger{

    public:
        static const std::string kLogLevelInfo; ///< string identifier for info logs.    
        static const std::string kLogLevelError; ///< string identifier for error logs.

        /// Returns a reference to the singleton Logger object
        ///
        /// @param filename: name of the file where to log
        /// @return singleton Logger instance
        static Logger* instance(const std::string& filename);

        /// Writes a message into the log file
        ///
        /// @param msg: message to write
        /// @return no return
        void log(const std::string& msg);

    protected:
        static std::vector<Logger*> vpInstance; ///< Ensemble of one-and-only instances
        static std::vector<std::string> vLogNames; ///< Ensemble of log filenames

        /// Embedded class to make sure the single Logger
        /// instance gets deleted on program shutdown.
        friend class Cleanup;
        class Cleanup{
            public:
                ~Cleanup();
        };

    private:
        std::string _filename;
        static std::mutex sMutex;
        Logger(const std::string& filename);
        virtual ~Logger();
        Logger(const Logger&);
        Logger& operator=(const Logger&);
};

#endif

