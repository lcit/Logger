/*	=========================================================================
	Author: Leonardo Citraro
	Company: DOS-group
	Filename: test_logger.cpp
	Last modifed: 23.05.2016 by Leonardo Citraro
	Description:	Testing the singleton logger class
	=========================================================================

	=========================================================================
*/
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "Logger.hpp"

using namespace std;

int main(int argc, char** argv){
	
	cout << "Testing the logger..." << endl;

	int i= 2;

	LOG_INFO("another info message " << i);
	LOG_ERROR("another error message " << i*3);
	LOG_CUSTOM("[CUSTOM]:custom message " << i*4);

	LOG_INFO("log.txt", "another info message " << i);
	LOG_ERROR("log.txt", "another error message " << i*3);
	LOG_CUSTOM("log.txt", "[CUSTOM]:custom message " << i*4);
	
	cout << "You should have some log files in your current folder" << endl;

	return 0;
}
