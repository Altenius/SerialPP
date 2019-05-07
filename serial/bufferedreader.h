#ifndef SERIAL_BUFFEREDREADER_H
#define SERIAL_BUFFEREDREADER_H

#include "device.h"
#include <vector>

namespace serial {
class BufferedReader {
public:
    explicit BufferedReader(Device &device) : device_(device) {}

    // Reads exactly `amount` bytes
    std::string read(int amount);

    // Reads a single line terminating in CR or LF
    std::string readLine();

    // Clears buffer
    inline void clear() noexcept { buffer_.clear(); }

private:
    std::vector<char> buffer_;
    Device &device_;

    void readSome();
};
}

#endif //LIBRETUNER_BUFFEREDREADER_H
