#include "../include/slcanCANable2.h"
#include "../include/CANableCommands.h"
#include <thread>
#include <chrono>
#include <stdexcept>


// Constructor
slcanCANable2::slcanCANable2(std::string channel, bool is_fd, int bitrate, int data_bitrate){
    _channel = channel;
    _is_fd = is_fd;
    _bitrate = bitrate;
    _data_bitrate = data_bitrate;

    _serialSetup();
}

// Destructor
slcanCANable2::~slcanCANable2(){
    // close CAN bus channel
    _write(CANableCommands::CLOSE_CHANNEL);
    // close serial port
    _serial.Close();
}

// Initial configuration
void slcanCANable2::_serialSetup(){

    // Serial port config
    _serial.Open(_channel); // Open serial port
    _serial.SetBaudRate(LibSerial::BaudRate::BAUD_115200); // Set tty baudrate
    std::this_thread::sleep_for(std::chrono::seconds(_SERIAL_OPEN_DELAY)); //sleep 2 seconds
    
    // CANable2 config
    setBitrate(_bitrate);
    _write(CANableCommands::OPEN_CHANNEL);
    if (_is_fd) {
        setDataBitrate(_data_bitrate);
    }
}

// Serial write
void slcanCANable2::_write(std::string str){
    // for every character in string
    for (char& c : str) {
        _serial << c; // write to serial port
    }
    _serial << _LINE_TERMINATOR;
}

// Serial read
std::string slcanCANable2::_read(){
    size_t bytes_available = _serial.rdbuf() -> in_avail();
    char next_char;
    std::string buffer;

    for (int _ = 0; _ < bytes_available; _++) {
        _serial.get(next_char);
        if (next_char == _LINE_TERMINATOR) {
            break;
        } else {
            buffer.push_back(next_char);
        }
    }
    return buffer;
}

// Public methods

// Set CAN bitrate
void slcanCANable2::setBitrate(int bitrate) {
    switch (bitrate)
    {
    case 10000:_write(CANableCommands::BITRATE_10K);
        break;
    case 20000:_write(CANableCommands::BITRATE_20K);
        break;
    case 50000:_write(CANableCommands::BITRATE_50K);
        break;
    case 100000:_write(CANableCommands::BITRATE_100K);
        break;
    case 125000:_write(CANableCommands::BITRATE_125K);
        break;
    case 250000:_write(CANableCommands::BITRATE_250K);
        break;
    case 500000:_write(CANableCommands::BITRATE_500K);
        break;
    case 750000:_write(CANableCommands::BITRATE_750K);
        break;
    case 1000000:_write(CANableCommands::BITRATE_1M);
        break;
    case 83300:_write(CANableCommands::BITRATE_83_3K);
        break;
    default: 
        throw std::runtime_error("Invalid bitrate value");
        break;
    }
}

// Set CAN data bitrate (CAN FD only)
void slcanCANable2::setDataBitrate(int data_bitrate) {
    switch (data_bitrate)
    {
    case 2000000:_write(CANableCommands::DATA_BITRATE_2M);
        break;
    case 5000000:_write(CANableCommands::DATA_BITRATE_5M);
        break;
    default:
        throw std::runtime_error("Invalid data bitrate value");
        break;
    }
}

std::string slcanCANable2::getVersion(){
    std::string version;
    _write(CANableCommands::GET_VERSION);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    version = _read();
    return version;
}