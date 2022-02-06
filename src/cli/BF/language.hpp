#ifndef CLI_BF_LANGUAGE_HPP_
#define CLI_BF_LANGUAGE_HPP_

#include <argparser.hpp>
#include <iostream>
#include <memory>

#include "../../common/debugprint.hpp"
#include "../language.hpp"
#include "except.hpp"
#include "interpreter.hpp"
#include "memory.hpp"
#include "preprocessor.hpp"
#include "program.hpp"

class LangBF : public Language
{
 private:
    argp::ArgParser parser;
    bool default_b;
    std::string default_s;
    int default_i;

    void print_help(const char *prog_name);

    void process_options();
    void setup_program();
    void setup_preprocessor();
    void setup_interpreter();

    BF::MemDataType mem_data_type;
    BF::InterFactory inter_fact;
    BF::MemFactory mem_fact;
    BF::Program prog;
    BF::Preprocessor prep;
    std::unique_ptr<BF::InterpreterBase> inter;

 public:
    LangBF();

    virtual bool load_options(int argc, const char **argv) override;
    virtual void prepare() override;
    virtual void run() override;
};

#endif // CLI_BF_LANGUAGE_HPP_
