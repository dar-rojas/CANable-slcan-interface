#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <cstdint>

struct DataFrame {
    uint32_t id; // data frame id must be 11 or 29 bits
    bool extended_id = false; // if the id is extended (29 bits)
    bool is_fd = false; // if the data frame is a flexible data frame
    bool is_rx = false; // if the data frame is a recibed dataframe (rx) or transmited dataframe (tx)
    bool is_remote = false; // if the data frame is a remote data frame (request data from another node)
    bool is_brs = false; // Bit Rate Switch, enables switching to a higher data bitrate for the data phase (CAN FD only)
    int dlc = 0; // Data Length Code, number of data bytes in the dataframe 0-8 for standard CAN and 0-32 for CAN FD
    uint8_t data[64];
};

#endif // DATA_FRAME_H