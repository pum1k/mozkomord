#include "language.hpp"

void LanguageBase::execute(int argc, const char **argv)
{
    if (!this->load_options(argc, argv))
    {
        return;
    }
    this->prepare();
    this->run();
}
