#ifndef CLI_PLATFORM_SETUP_HPP_
#define CLI_PLATFORM_SETUP_HPP_

#if !defined(_WIN32)
#    warning This platform is not supported or was not recognised. \
This means no platform dependent setup will be made.
#    define CLI_PLATFORM_UNRECOGNISED 1
#endif // unrecognised platforms

#include "../../common/debugprint.hpp"

#ifdef _WIN32
#    include <windows.h>

#    include <cstdint>
#endif // _WIN32

class ConsoleSetup
{
 private:
#ifdef _WIN32
    uint64_t console_out_cp_;
#endif // _WIN32
 public:
    ConsoleSetup();
    ~ConsoleSetup();
};

#endif // CLI_PLATFORM_SETUP_HPP_
