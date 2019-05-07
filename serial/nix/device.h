#include <utility>

#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include <string>

#include "../settings.h"

namespace serial {
class Device {
public:
    explicit Device(std::string port = "", Settings settings = Settings{}) : port_(std::move(port)), settings_{settings} {}

    void open();

    void close();

    inline void setPort(const std::string &port) noexcept { port_ = port; }

    inline void setSettings(Settings settings) noexcept { settings_ = settings; }

    inline Settings settings() const noexcept { return settings_; }

    inline bool isOpen() const noexcept { return fd_ != -1; }

    void updateSettings();

private:
    int fd_{-1};
    std::string port_;
    Settings settings_;
};
}


#endif //SERIAL_DEVICE_H
