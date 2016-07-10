Thread-safe FIFO buffer written in C++

Using std::unique_ptr<ITEM> as FIFO type is good practice if your ITEM is big in size.

/// Example usage:
///
///     FIFO<std::unique_ptr<float>, FIFOdumpTypes::DumpNewItem> fifo(5);
///     std::unique_ptr<float> temp = std::make_unique<float>(2.1);	
///     fifo.push(temp);
///     int size = fifo.size();
///     fifo.pull(temp);

The derived class sFIFO is intended to be used with frames that are measured in seconds:

/// Example usage:
///
///     class ITEM {
///     	public:
///     		std::string _value;
///             const float _size_seconds = 1.2;
///     		ITEM(const std::string& value):_value(value){}
///     		float get_size_seconds(){return _size_seconds;}
///     };
///
///     sFIFO<std::unique_ptr<ITEM>, FIFOdumpTypes::DumpNewItem> fifo(5);
///     std::unique_ptr<ITEM> temp = std::make_unique<ITEM>("an item");	
///     fifo.push(temp);
///     float size = fifo.size(); // <-- this value is in seconds
///     fifo.pull(temp);
