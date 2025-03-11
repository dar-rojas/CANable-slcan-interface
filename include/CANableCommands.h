#ifndef CANABLE_COMMANDS_H
#define CANABLE_COMMANDS_H

#include <string>

// Namespace to group all CANable slcan firmware commands
namespace CANableCommands {
    // Channel control commands
    const std::string OPEN_CHANNEL = "O";  // Open channel
    const std::string CLOSE_CHANNEL = "C"; // Close channel

    // Nominal bitrate configuration commands
    const std::string BITRATE_10K = "S0";   // Set nominal bitrate to 10k
    const std::string BITRATE_20K = "S1";   // Set nominal bitrate to 20k
    const std::string BITRATE_50K = "S2";   // Set nominal bitrate to 50k
    const std::string BITRATE_100K = "S3";  // Set nominal bitrate to 100k
    const std::string BITRATE_125K = "S4";  // Set nominal bitrate to 125k
    const std::string BITRATE_250K = "S5";  // Set nominal bitrate to 250k
    const std::string BITRATE_500K = "S6";  // Set nominal bitrate to 500k
    const std::string BITRATE_750K = "S7";  // Set nominal bitrate to 750k
    const std::string BITRATE_1M = "S8";    // Set nominal bitrate to 1M
    const std::string BITRATE_83_3K = "S9"; // Set nominal bitrate to 83.3k

    // Data bitrate configuration commands (CAN FD only)
    const std::string DATA_BITRATE_2M = "Y2"; // Set data bitrate to 2M(default)
    const std::string DATA_BITRATE_5M = "Y5"; // Set data bitrate to 5M

    // Mode configuration commands
    const std::string MODE_NORMAL = "M0"; // Set mode to normal mode (default)
    const std::string MODE_SILENT = "M1"; // Set mode to silent mode

    // Automatic retransmission configuration commands
    const std::string AUTO_RETRANSMISSION_OFF = "A0"; // Disable automatic retransmission
    const std::string AUTO_RETRANSMISSION_ON = "A1";  // Enable automatic retransmission (default)

    // Information commands
    const std::string GET_VERSION = "V"; // Returns firmware version and remote path as a string
    const std::string GET_ERROR_REGISTER = "E"; // Returns the error register
}

#endif // CANABLE_COMMANDS_H