#ifndef CLI_UTILS_PROGRAM_HPP_
#define CLI_UTILS_PROGRAM_HPP_

#include <fstream>
#include <string>
#include <vector>

#include "except.hpp"

namespace utils
{

/**
 * This class is responsible for loading and storing programs.
 * It also defines types that are used to store the program.
 */
class Program
{
 public:
    using value_type = unsigned char;
    using container  = std::vector<value_type>;

 private:
    container prog;

 public:
    Program(){};
    Program(const std::string &prog);
    Program(const container &prog);

    void set(const std::string &prog);
    void set(const container &prog);
    void load(const std::string &file);

    container &get();
    const container &get_const() const;
};

} // namespace utils

#endif // CLI_UTILS_PROGRAM_HPP_
