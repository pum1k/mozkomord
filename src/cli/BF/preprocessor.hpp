#ifndef CLI_BF_PREPROCESSOR_HPP_
#define CLI_BF_PREPROCESSOR_HPP_

#include <vector>

class BFPreprocessor
{
 protected:
    std::vector<char> program;

 public:
    virtual ~BFPreprocessor() {}

    void set_program(const std::vector<char> &prog);
    virtual const std::vector<char> &get_program();

    virtual void check()                         = 0;
    virtual void process(bool keep_debug = true) = 0;
};

#endif // CLI_BF_PREPROCESSOR_HPP_
