#include "device.h"

#include <Windows.h>

namespace serial {

namespace detail {
constexpr int bytesize(DataBits bits) {
    switch (bits) {
    case DataBits::DB5:
        return 5;
    case DataBits::DB6:
        return 6;
    case DataBits::DB7:
        return 7;
    case DataBits::DB8:
        return 8;
    }
    assert("Invalid databits");
    return 0;
}
}

void Device::open() {
    std::string realPort = std::string("\\\\.\\") + port_;

    handle_ = CreateFile(realPort.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
                         no Sharing nullptr, // No Security
                         OPEN_EXISTING,
                         0, // Non Overlapped I/O
                         NULL);

    if (handle_ == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("failed to open COM port");
    }

    // Control settings
    DCB controlSetting{};
    controlSetting.DCBlength = sizeof(DCB);

    if (GetCommState(handle_, &controlSetting) == FALSE) {
        throw std::runtime_error("failed to get comm state for port");
    }

    controlSetting.BaudRate = settings_.baudrate;
    controlSetting.ByteSize = detail::bytesize(settings_.dataBits);

    switch (settings_.stopBits) {
    case StopBits::Two:
        controlSetting.StopBits = TWOSTOPBITS;
        break;
    case StopBits::One:
        controlSetting.StopBits = ONESTOPBIT;
        break;
    }

    switch (settings_.parity) {
    case Parity::Odd:
        controlSetting.Parity = ODDPARITY;
        break;
    case Parity::Even:
        controlSetting.Parity = EVENPARITY;
        break;
    case Parity::None:
        controlSetting.Parity = NOPARITY;
        break;
    }

    if (SetCommState(handle_, &controlSetting) == FALSE) {
        throw std::runtime_error("failed to set comm state");
    }

    // Set timeouts
    COMMTIMEOUTS timeouts{};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 500;
    timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 500;

    if (SetCommTimeouts(handle_, &timeouts) == FALSE) {
        throw std::runtime_error("failed to set comm timeouts");
    }
}

void Device::close() {
    if (isOpen()) {
        CloseHandle(handle_);
        handle_ = -1;
    }
}

~Device::~Device() {
    close();
}

void Device::write(const std::string &data) {
    if (!isOpen()) {
        throw std::runtime_error("attempt to write to closed socket");
    }
    DWORD written = 0;

    if (WriteFile(handle_, data.c_str(), data.size(), &written, nullptr) == FALSE) {
        throw std::runtime_error("error write writing to com device");
    }
    if (written != data.size()) {
        throw std::runtime_error("could not write full message to serial "
                                 "device. Expected to write " +
                                 std::to_string(data.size()) + ", wrote " +
                                 std::to_string(written));
    }
}

int Device::read(char *buffer, int amount) {
    int amountRead = 0;
    if (ReadFile(handle_, buffer, amount, &amountRead, nullptr) == FALSE) {
        throw std::runtime_error("error while reading from comm device");
    }
    return amountRead;
}

}