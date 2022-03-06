#ifndef CLI_BF_PROGRAM_HPP_
#define CLI_BF_PROGRAM_HPP_

#include <fstream>
#include <string>
#include <vector>

#include "except.hpp"

namespace BF
{

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

} // namespace BF

#endif // CLI_BF_PROGRAM_HPP_
