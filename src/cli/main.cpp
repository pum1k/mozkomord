#include <iostream>
#include <memory>

#include "BF/language.hpp"
#include "language.hpp"
#include "platform/setup.hpp"

int main(int argc, const char *argv[])
{
    ConsoleSetup s;

    // At least one argument required
    if (argc < 2)
    {
        std::cerr << "ERROR: No options specified, exiting.\n"
                  << "You can use \"--help\" option to get information about "
                     "options.\n";
        return -1;
    }

    std::unique_ptr<LanguageBase> lang = nullptr;

    lang = std::make_unique<LangBF>();

    try
    {
        lang->execute(argc, argv);
    }
    catch (const debugger_terminate &e)
    {
        std::cerr << e.what() << '\n';
        return -2;
    }
    catch (const language_error &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "You can use \"--help\" option to get information about "
                     "options.\n";
        return 1;
    }

    return 0;
}
