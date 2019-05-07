#include "bufferedreader.h"

#include <stdexcept>
#include <array>
#include <algorithm>

namespace serial {


void BufferedReader::readSome() {
    int preSize = buffer_.size();
    buffer_.resize(buffer_.size() + 1024);

    int amountRead = device_.read(buffer_.data() + preSize, 1024);
    buffer_.resize(preSize + amountRead);

    if (amountRead == 0) {
        throw std::runtime_error("received 0 bytes from socket");
    }
}

std::string BufferedReader::read(int amount) {
    while (buffer_.size() < amount) {
        readSome();
    }
    std::string res(buffer_.begin(), buffer_.begin() + amount);
    buffer_.erase(buffer_.begin(), buffer_.begin() + amount);
    return res;
}

std::string BufferedReader::readLine() {
    constexpr std::array<char, 2> terminators{'\r', '\n'};

    auto pos = std::find_first_of(buffer_.end(), buffer_.end(), terminators.begin(), terminators.end());
    while (pos == buffer_.end()) {
        std::size_t size = buffer_.size();
        readSome();
        // Start searching from where we left off
        pos = std::find_first_of(buffer_.begin() + size, buffer_.end(), terminators.begin(), terminators.end());
    }

    std::string line(buffer_.begin(), pos);
    std::advance(pos, 1);
    // Clear trailing whitespace
    if (pos != buffer_.end() && (*pos== '\r' || *pos == '\n')) {
        std::advance(pos, 1);
    }
    buffer_.erase(buffer_.begin(), pos + 1);

    return line;
}
}