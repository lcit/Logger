# =========================================================================
# Author: Leonardo Citraro
# Company: 
# Filename: Makefile	
# =========================================================================
# /////////////////////////////////////////////////////////////////////////
CPP         = g++
INCLUDES    = -I.
CPPFLAGS    = -std=c++14 -pthread  $(INCLUDES)
DEPS        = 
OBJS        = 
LDFLAGS     = -g $(DEPS)
# /////////////////////////////////////////////////////////////////////////

all: Logger.o test_Logger 

Logger.o: Logger.cpp
	$(CPP) $(CPPFLAGS) -c Logger.cpp $(OBJS) $(LDFLAGS)
test_Logger: test_Logger.cpp
	$(CPP) $(CPPFLAGS) -o test_Logger test_Logger.cpp ./Logger.o $(OBJS) $(LDFLAGS)

clean:
	-rm -f *.o; rm test_Logger
