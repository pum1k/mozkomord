#include "optimizer.hpp"

namespace BF
{
template <class iterator,
          std::enable_if_t<std::is_same_v<typename iterator::value_type,
                                          Program::value_type>,
                           bool> = true>
void uint_to_prog(iterator &inst_ptr, uint16_t num)
{
    for (std::size_t i = 0; i < sizeof(uint16_t); i++)
    {
        *(++inst_ptr) = num & 0xff;
        num >>= 8;
    }
}

template <class iterator,
          std::enable_if_t<std::is_same_v<typename iterator::value_type,
                                          Program::value_type>,
                           bool> = true>
uint16_t prog_to_uint(iterator &inst_ptr)
{
    uint16_t rv = 0;
    uint16_t tmp;

    for (std::size_t i = 0; i < sizeof(uint16_t); i++)
    {
        tmp = *(++inst_ptr);
        rv += tmp << (8 * i);
    }

    return rv;
}

} // namespace BF
