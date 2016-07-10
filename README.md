## Lightweight thread-safe singleton logger written in C++

You just need to add `#include<Logger.hpp>` into your code and enjoy logging!

```
Example usage:

	LOG_CUSTOM("another custom  message for log2.txt");
	LOG_CUSTOM("log2.txt", "another custom  message for log2.txt");
	LOG_ERROR("log1.txt", "an error message for log.txt");
```
