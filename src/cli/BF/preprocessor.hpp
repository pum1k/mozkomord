#ifndef CLI_BF_PREPROCESSOR_HPP_
#define CLI_BF_PREPROCESSOR_HPP_

#include <functional>
#include <vector>

#include "except.hpp"

namespace BF
{
class Preprocessor
{
 protected:
    std::vector<char> program;

    std::function<void(const std::vector<char> &)> check_;
    std::function<void(std::vector<char> &, bool)> process_;

 public:
    void set_program(const std::vector<char> &prog);
    const std::vector<char> &get_program();

    void set_check(const std::function<void(const std::vector<char> &)> &func);
    void set_process(
        const std::function<void(std::vector<char> &, bool)> &func);

    void check();
    void process(bool keep_debug = true);
};

void standard_check(const std::vector<char> &prog);

} // namespace BF

#endif // CLI_BF_PREPROCESSOR_HPP_
