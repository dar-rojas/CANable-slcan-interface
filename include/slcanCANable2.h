#ifndef SLCAN_CANABLE2_H
#define SLCAN_CANABLE2_H

#include <DataFrame.h>
#include <libserial/SerialStream.h>
#include <string>

int const _SERIAL_OPEN_DELAY = 2; //delay in seconds after serial open
char const _LINE_TERMINATOR = '\r';

/// @brief Interface for CANable with slcan firmware
class slcanCANable2
{
private:
    LibSerial::SerialStream _serial;
    std::string _channel;
    bool _is_fd;
    int _bitrate;
    int _data_bitrate;

    void _write(std::string str);
    
    void _serialSetup();
    char _getFrameTypeChar(DataFrame frame);
    char _getFrameLengthChar(DataFrame frame);
    void _uint8ToHexPair(uint8_t number, char* output);
    void _uint32ToHexPair(uint32_t number, char* output);

public:
    slcanCANable2(std::string channel, bool is_fd, int bitrate, int data_bitrate);
    ~slcanCANable2();
    void setBitrate(int bitrate = 0);
    void setDataBitrate(int data_bitrate = 0);
    void send(DataFrame frame);
    std::string _read();
    std::string getVersion();
};

#endif // SLCAN_CANABLE2_H