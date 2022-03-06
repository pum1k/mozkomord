#ifndef CLI_BF_STRINGS_HPP_
#define CLI_BF_STRINGS_HPP_

// #############################################################################
// #### MEMORY ERROR MESSAGES ##################################################
// #############################################################################

namespace BF
{

namespace strings
{

inline constexpr const char *increment_error =
    "Incrementing pointer to the last element in the memory is not allowed for "
    "this type of memory. "
    "Try increasing the size of the memory or using a different memory type.";

inline constexpr const char *decrement_error =
    "Decrementing pointer to the first element in the memory is not allowed "
    "for this type of memory. "
    "Try using a different memory type.";

inline constexpr const char *prog_read_input_error =
    "Error while reading input for the interpreted program.";

inline constexpr const char *mem_ptr_null_error =
    "Memory pointer cannot be nullptr.";

inline constexpr const char *mem_index_error =
    "Memory does not contain element with index: ";

} // namespace strings

} // namespace BF

#endif // CLI_BF_STRINGS_HPP_
