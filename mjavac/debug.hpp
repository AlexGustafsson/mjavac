#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#ifdef DEBUG
#define debug_out std::cerr
#else
#define debug_out false && std::cerr
#endif

#endif
