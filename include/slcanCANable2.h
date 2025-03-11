#ifndef SLCAN_CANABLE2_H
#define SLCAN_CANABLE2_H

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
    std::string _read();
    void _serialSetup();

public:
    slcanCANable2(std::string channel, bool is_fd, int bitrate, int data_bitrate);
    ~slcanCANable2();
    void setBitrate(int bitrate = 0);
    void setDataBitrate(int data_bitrate = 0);
    std::string getVersion();
};

#endif // SLCAN_CANABLE2_H