# C++ - Lightweight thread-safe singleton logger

### Usage
Just include the header `#include "Logger.hpp"` in your source file!

### Prerequisites

### Example usage

```C++
#include "Logger.hpp"
int main(int argc, char** argv){

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
```
The resulting files:
```
// log
[2016-12-30/07:26:38][INFO]:	a log message
[2016-12-30/07:26:38][ERROR]:	another log message
[2016-12-30/07:26:38]a custom log message
[2016-12-30/07:26:38][INFO]:	a log message 2 cont'd abcdef
[2016-12-30/07:26:38][ERROR]:	another log message 6 cont'd abcdef
[2016-12-30/07:26:38]a custom log message 8 cont'd abcdef

// another.txt
[2016-12-30/07:26:38][INFO]:	a log message 2 cont'd abcdef
[2016-12-30/07:26:38][ERROR]:	another log message 6 cont'd abcdef
[2016-12-30/07:26:38]a custom log message 8 cont'd abcdef
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments
