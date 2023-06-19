#ifndef CLI_BF_LANGUAGE_HPP_
#define CLI_BF_LANGUAGE_HPP_

#include <argparser.hpp>
#include <iostream>
#include <memory>

#include "../../common/debugprint.hpp"
#include "../language.hpp"
#include "../utils/preprocessor.hpp"
#include "../utils/program.hpp"
#include "except.hpp"
#include "interpreter.hpp"
#include "memory.hpp"
#include "preprocessor.hpp"

template <>
void argp::KeywordOption<BF::MemoryType>::from_string(
    const std::vector<std::string_view> &strings);

template <>
void argp::KeywordOption<BF::MemDataType>::from_string(
    const std::vector<std::string_view> &strings);

/**
 * This class is responsible for setting up and running interpretation of BF
 * programs.
 * It parses command line arguments, sets up the interpreter and all other
 * required objects and then runs the interpreter.
 */
class LangBF : public LanguageBase
{
 protected:
    // check if command line options are valid, update inner classes accordingly
    virtual void process_options();
    // load file to prog object
    virtual void setup_program();
    // set functions for preprocessing the BF code
    virtual void setup_preprocessor();
    // create interpreter object
    virtual void setup_interpreter();

    argp::PositionalOption<std::string> filename;
    argp::KeywordOption<BF::MemoryType> mem_type;
    argp::KeywordOption<int> mem_size;
    argp::KeywordOption<BF::MemDataType> mem_cell_size;
    argp::KeywordOption<bool> debug;
    argp::KeywordOption<bool> optimize;
    argp::KeywordOption<bool> help;
    argp::OptionsList opts;

    BF::MemDataType mem_data_type;
    BF::InterFactory inter_fact;
    BF::MemFactory mem_fact;
    utils::Program prog;
    utils::Preprocessor prep;
    std::unique_ptr<BF::InterpreterBase> inter;

    virtual bool load_options(int argc, const char **argv) override;
    virtual void prepare() override;
    virtual void run() override;

 public:
    LangBF();
};

#endif // CLI_BF_LANGUAGE_HPP_
