#include "setup.hpp"

ConsoleSetup::ConsoleSetup()
{
#ifdef _WIN32
    // SO question about displaying UTF-8 on windows
    // https://stackoverflow.com/a/35576741
    // MS Docs for GetConsoleOutputCP
    // https://docs.microsoft.com/en-us/windows/console/getconsoleoutputcp
    this->console_out_cp_ = GetConsoleOutputCP();
    DEBUG_PRINT("[Windows setup]: Changing output code page from: "
                << this->console_out_cp_ << " to 65001 (UTF-8).");
    SetConsoleOutputCP(65001);
#elif CLI_PLATFORM_UNRECOGNISED
    DEBUG_PRINT(
        "[Platform setup]: WARNING: Unsupported/unrecognised platform! "
        "No platform depandent setup done.");
#endif // _WIN32, CLI_PLATFORM_UNRECOGNISED
}

ConsoleSetup::~ConsoleSetup()
{
#ifdef _WIN32
    SetConsoleOutputCP(this->console_out_cp_);
    DEBUG_PRINT("[Windows setup]: Cleanup - output code page set back to: "
                << this->console_out_cp_);
#endif // _WIN32
}
