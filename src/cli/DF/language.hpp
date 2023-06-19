#ifndef CLI_DF_LANGUAGE_HPP_
#define CLI_DF_LANGUAGE_HPP_

#include <iostream>

#include "../BF/language.hpp"
#include "interpreter.hpp"
#include "preprocessor.hpp"

/**
 * This class is used to run DoubleFuck programs.
 */
class LangDF : public LangBF
{
 protected:
    virtual void setup_preprocessor() override;
    virtual void setup_interpreter() override;

 public:
    LangDF();
};

#endif // CLI_DF_LANGUAGE_HPP_
