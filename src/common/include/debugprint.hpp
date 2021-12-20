#ifndef COMMON_DEBUGPRINT_HPP_
#define COMMON_DEBUGPRINT_HPP_

#ifdef DEBUG
#    include <iostream>
#endif // DEBUG

#ifdef DEBUG
#    define DEBUG_PRINT(msg) std::cout << msg << std::endl
#else // DEBUG
#    define DEBUG_PRINT(msg)
#endif // DEBUG

#endif // COMMON_DEBUGPRINT_HPP
