/*  =========================================================================
    Author: Leonardo Citraro
    Company: 
    Filename: test_functional.cpp
    Last modifed: 30.12.2016 by Leonardo Citraro
    Description:    Functional test.

    =========================================================================

    =========================================================================
*/
#include "Logger.hpp"

int main(int argc, char** argv){

    // cleanup
    std::system("rm ./*.txt > /dev/null 2>&1; rm ./log* > /dev/null 2>&1");
    
    // log some simple messages
    LOG_INFO("a log message");
    LOG_ERROR("another log message");
    LOG_CUSTOM("a custom log message");

    // make use of the stream feature (it works at run-time!)
    int i = 2;
    std::string str = "abcdef";
    LOG_INFO("a log message " << i << " cont'd " << str);
    LOG_ERROR("another log message " << i*3 << " cont'd " << str);
    LOG_CUSTOM("a custom log message " << i*4<< " cont'd " << str);

    // save to another file
    LOG_INFO("another.txt", "a log message " << i << " cont'd " << str);
    LOG_ERROR("another.txt", "another log message " << i*3 << " cont'd " << str);
    LOG_CUSTOM("another.txt", "a custom log message " << i*4<< " cont'd " << str);

    return 0;
}
