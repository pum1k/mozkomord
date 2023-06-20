#include "optimizer.hpp"

namespace BF
{

namespace optimizers
{

// helper function for inserting the optimized instructions and removing the
// original ones
void replace_consecutive_operations_with_command(
    utils::Program::value_type prev, int streak,
    std::list<utils::Program::value_type> &prog_l,
    std::list<utils::Program::value_type>::iterator &streak_begin_it,
    std::list<utils::Program::value_type>::iterator &it)
{
    utils::Program::value_type instr;
    switch (prev)
    {
    case '+':
        instr = static_cast<utils::Program::value_type>(op_multi_inc);
        break;
    case '-':
        instr = static_cast<utils::Program::value_type>(op_multi_dec);
        break;
    case '<':
        instr = static_cast<utils::Program::value_type>(op_multi_ptr_dec);
        break;
    case '>':
        instr = static_cast<utils::Program::value_type>(op_multi_ptr_inc);
        break;
    }

    *streak_begin_it = instr;
    utils::uint_to_prog(streak_begin_it, streak);
    ++streak_begin_it;
    prog_l.erase(streak_begin_it, it);
}

void remove_consecutive_operations(
    std::list<utils::Program::value_type> &prog_l)
{
    using iter_t = std::list<utils::Program::value_type>::iterator;

    iter_t it  = prog_l.begin();
    iter_t eit = prog_l.end();

    uint16_t streak                 = 0;
    utils::Program::value_type prev = *it;

    iter_t streak_begin_it;

    ++it;

    // search for streaks of 3 or more instructions +-<> and replace them with
    // optimized instructions
    for (; it != eit; ++it)
    {
        if (*it == prev)
        {
            if (streak != 0)
            {
                ++streak;
            }
            else if ((*it == '+') || (*it == '-') || (*it == '<') ||
                     (*it == '>'))
            {
                streak = 2;

                streak_begin_it = it;
                --streak_begin_it;
            }
        }
        else if (streak != 0)
        {
            if (streak > 2)
            {
                replace_consecutive_operations_with_command(
                    prev, streak, prog_l, streak_begin_it, it);
            }

            streak = 0;
        }
        prev = *it;
    }

    if (streak > 2)
    {
        replace_consecutive_operations_with_command(prev, streak, prog_l,
                                                    streak_begin_it, it);
    }

    streak = 0;
}

void replace_set_zero(std::list<utils::Program::value_type> &prog_l)
{
    using iter_t = std::list<utils::Program::value_type>::iterator;

    int length = 0;

    iter_t pattern_start_it;

    for (iter_t it = prog_l.begin(), eit = prog_l.end(); it != eit; ++it)
    {
        if (length == 3)
        {
            *pattern_start_it = op_set_zero;
            ++pattern_start_it;
            prog_l.erase(pattern_start_it, it);

            length = 0;
        }

        if (length == 0 && *it == '[')
        {
            ++length;
            pattern_start_it = it;
        }
        else if (length == 1 && (*it == '-' || *it == '+'))
        {
            ++length;
        }
        else if (length == 2 && *it == ']')
        {
            ++length;
        }
        else
        {
            length = 0;
        }
    }
}

} // namespace optimizers

} // namespace BF
