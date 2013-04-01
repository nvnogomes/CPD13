#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

#include "definitions.h"

/**
 * @brief debug_info
 * Prints debug messages when the
 * debug flag is true.
 *
 * @param format string to print
 * @return exit_code
 */
int
debug_info(const char *format, ...)
{
    int done = 0;

    if( DEBUG )
    {
        va_list arg;
        va_start (arg, format);
        done = vfprintf (stdout, format, arg);
        va_end (arg);
    }

    return done;
}
