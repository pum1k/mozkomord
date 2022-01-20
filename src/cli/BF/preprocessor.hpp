#ifndef CLI_BF_PREPROCESSOR_HPP_
#define CLI_BF_PREPROCESSOR_HPP_

#include <algorithm>
#include <functional>

#include "except.hpp"
#include "program.hpp"

namespace BF
{
class Preprocessor
{
 protected:
    std::function<void(const Program::container &)> check_;
    std::function<void(Program::container &, bool)> process_;

 public:
    void set_check(const std::function<void(const Program::container &)> &func);
    void set_process(
        const std::function<void(Program::container &, bool)> &func);

    void check(Program &prog);
    void process(Program &prog, bool keep_debug = true);
};

void standard_check(const Program::container &prog);

namespace prep
{
void noop(Program::container &, bool);
void remove_unused(Program::container &prog, bool keep_debug);
} // namespace prep

} // namespace BF

#endif // CLI_BF_PREPROCESSOR_HPP_
