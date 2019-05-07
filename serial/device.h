#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#ifdef __unix__
#include "nix/device.h"
#elif WIN32
#include "win32/device.h"
#endif

#endif //SERIAL_DEVICE_H
