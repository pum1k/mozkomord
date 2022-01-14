#ifndef CLI_PLATFORM_SETUP_HPP_
#define CLI_PLATFORM_SETUP_HPP_

#if !defined(WIN32)
#    warning This platform is not supported or was not recognised. \
This means no platform dependent setup will be made.
#    define CLI_PLATFORM_UNRECOGNISED 1
#endif // unrecognised platforms

#include "../../common/debugprint.hpp"

#ifdef WIN32
#    include <windows.h>

#    include <cstdint>
#endif // WIN32

class ConsoleSetup
{
 private:
#ifdef WIN32
    uint64_t console_out_cp_;
#endif // WIN32
 public:
    ConsoleSetup();
    ~ConsoleSetup();
};

#endif // CLI_PLATFORM_SETUP_HPP_
