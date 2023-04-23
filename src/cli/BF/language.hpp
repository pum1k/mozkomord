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

/**
 * This class is responsible for setting up and running interpretation of BF
 * programs.
 * It parses command line arguments, sets up the interpreter and all other
 * required objects and then runs the interpreter.
 */
class LangBF : public LanguageBase
{
 protected:
    argp::ArgParser parser;
    bool default_b;
    std::string default_s;
    int default_i;

    void print_help(const char *prog_name);

    // check if command line options are valid, store some of them outside the
    // parser object
    void process_options();
    // load file to prog object
    void setup_program();
    // set fuctions for preprocessing the BF code
    void setup_preprocessor();
    // create interpreter object
    void setup_interpreter();

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
