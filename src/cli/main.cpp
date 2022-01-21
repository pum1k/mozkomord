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

    std::unique_ptr<Language> lang = nullptr;

    lang = std::make_unique<LangBF>();

    try
    {
        if (!lang->load_options(argc - 1, &argv[1]))
        {
            return 0;
        }
        lang->prepare();
        lang->run();
    }
    catch (const language_error &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "You can use \"--help\" option to get information about "
                     "options.\n";
    }

    return 0;
}
