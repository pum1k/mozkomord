#include "setup.hpp"

ConsoleSetup::ConsoleSetup()
{
#ifdef WIN32
    // SO question about displaying UTF-8 on windows
    // https://stackoverflow.com/a/35576741
    // MS Docs for GetConsoleOutputCP
    // https://docs.microsoft.com/en-us/windows/console/getconsoleoutputcp
    this->console_out_cp_ = GetConsoleOutputCP();
    DEBUG_PRINT("[Windows setup]: Changing output code page from: "
                << this->console_out_cp_ << " to 65001 (UTF-8).");
    SetConsoleOutputCP(65001);
#endif // WIN32
}

ConsoleSetup::~ConsoleSetup()
{
#ifdef WIN32
    SetConsoleOutputCP(this->console_out_cp_);
#endif // WIN32
}
