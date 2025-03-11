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
    if (_is_fd) {
        setDataBitrate(_data_bitrate);
    }
    _write(CANableCommands::OPEN_CHANNEL);
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
    char next_char;
    std::string buffer;

    int timeout = 10;
    auto start_time = std::chrono::steady_clock::now();

    while(true) {

        // elapsed time
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start_time
        ).count();

        _serial.get(next_char);
        if (next_char == _LINE_TERMINATOR) {
            break;
        } else if (elapsed_time >= timeout) {
            break;
        } else {
            buffer.push_back(next_char);
        }
    }
    return buffer;
}

// Get frame type char
char slcanCANable2::_getFrameTypeChar(DataFrame frame) {

    /* Frame type
    * type code | is_df | is_extended | is_remote | is_brs |
    *      t    |   0   |      0      |     0     |    0   | 0x00
    *      T    |   0   |      1      |     0     |    0   | 0x04
    *      R    |   0   |      1      |     1     |    0   | 0x06
    *      r    |   0   |      0      |     1     |    0   | 0x02
    *      d    |   1   |      0      |     0     |    0   | 0x08
    *      D    |   1   |      1      |     0     |    0   | 0x0C
    *      b    |   1   |      0      |     0     |    1   | 0x09
    *      B    |   1   |      1      |     0     |    1   | 0x0D
    */
    uint8_t is_df = frame.is_fd;
    uint8_t is_extended = frame.extended_id;
    uint8_t is_remote = frame.is_remote;
    uint8_t is_brs = frame.is_brs;

    uint8_t type = (is_df << 3) | (is_extended << 2) | (is_remote << 1) | is_brs;

    char type_char;

    switch (type)
    {
    case 0x00 : type_char = 't';
        break;
    case 0x04 : type_char = 'T';
        break;
    case 0x06 : type_char = 'R';
        break;
    case 0x02 : type_char = 'r';
        break;
    case 0x08 : type_char = 'd';
        break;
    case 0x0C : type_char = 'D';
        break;
    case 0x09 : type_char = 'b';
        break;
    case 0x0D : type_char = 'B';
        break;
    default:
        throw std::runtime_error("Invalid data frame");
    }
    
    return type_char;
}

// Get data length char
char slcanCANable2::_getFrameLengthChar(DataFrame frame) {

    char length_char;
    int dlc = frame.dlc;

    if ( dlc >= 0 && dlc <= 8 ) {
        length_char = char(dlc+48); // number as chars
    } else {
        switch (dlc)
        {
        case 12 : length_char = '9';
            break;
        case 16 : length_char = 'A';
            break;
        case 20 : length_char = 'B';
            break;
        case 24 : length_char = 'C';
            break;
        case 32 : length_char = 'D';
            break;
        case 48 : length_char = 'E';
            break;
        case 64 : length_char = 'F';
            break;
        default:
            throw std::runtime_error("Invalid DLC");
        }
    }

    return length_char;
}

// uint8_t to Hex pair converter

void slcanCANable2::_uint8ToHexPair(uint8_t number, char* output) {
    const char hex_chars[] = "0123456789ABCDEF";
    output[0] = hex_chars[(number >> 4) & 0x0F];
    output[1] = hex_chars[number & 0x0F];
}

// uint32 to Hex pairs converter
void slcanCANable2::_uint32ToHexPair(uint32_t number, char* output) {
    const char hex_chars[] = "0123456789ABCDEF";
    output[0] = hex_chars[(number >> 12) & 0x0F];
    output[1] = hex_chars[(number >> 8) & 0x0F];
    output[2] = hex_chars[(number >> 4) & 0x0F];
    output[3] = hex_chars[number & 0x0F];
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

void slcanCANable2::send(DataFrame frame){
    std::string can_message;
    char type_char = _getFrameTypeChar(frame);
    char lenght_char = _getFrameLengthChar(frame);

    can_message.push_back(type_char); // add type char

    // add id chars
    char id_chars[4];
    _uint32ToHexPair(frame.id, id_chars);
    can_message.push_back(id_chars[0]);
    can_message.push_back(id_chars[1]);
    if (frame.extended_id) {
        can_message.push_back(id_chars[2]);
        can_message.push_back(id_chars[3]);
    }

    // add length char
    can_message.push_back(lenght_char);

    // add data chars
    for (int i = 0; i < frame.dlc; i++) {
        char data_chars[2];
        _uint8ToHexPair(frame.data[i], data_chars);
        can_message.push_back(data_chars[0]);
        can_message.push_back(data_chars[1]);
    }
    
    _write(can_message);
}