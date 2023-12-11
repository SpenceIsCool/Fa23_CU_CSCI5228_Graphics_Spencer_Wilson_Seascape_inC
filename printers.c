#include "SDW.h"

// PRINTERS

// flood, stream, trickle, none
const char _DEBUG_LEVEL[]="none";

/**
 * printStuff
 *
 * use to print anything to the terminal, not the display
 * credit: https://piazza.com/class/lls99rbz11v578/post/10
 */
    __attribute__((format(printf, 1, 2)))
void printStuff(char const* f, ...)
{
    va_list args;
    va_start(args, f);
    vprintf(f, args);
    va_end(args);
}

/**
 * debugFlood
 *
 * only print stuff if _DEBUG_LEVEL is set to "flood"
 * credit: https://piazza.com/class/lls99rbz11v578/post/10
 */
    __attribute__((format(printf, 1, 2)))
void debugFlood(char const* f, ...)
{
    if (!strcmp(_DEBUG_LEVEL, "flood"))
    {
        va_list args;
        va_start(args, f);
        vprintf(f, args);
        va_end(args);
    }
}

/**
 * debugStream
 *
 * only print stuff if _DEBUG_LEVEL is set to "flood" OR "stream"
 * credit: https://piazza.com/class/lls99rbz11v578/post/10
 */
    __attribute__((format(printf, 1, 2)))
void debugStream(char const* f, ...)
{
    if (!strcmp(_DEBUG_LEVEL, "stream") || !strcmp(_DEBUG_LEVEL, "flood"))
    {
        va_list args;
        va_start(args, f);
        vprintf(f, args);
        va_end(args);
    }
}

/**
 * debugTrickle
 *
 * only print stuff if _DEBUG_LEVEL is set to "flood" OR "stream" OR "trickle"
 * recomend setting _DEBUG_LEVEL to "none" if you don't want to print debug messages
 *
 * credit: https://piazza.com/class/lls99rbz11v578/post/10
 */
    __attribute__((format(printf, 1, 2)))
void debugTrickle(char const* f, ...)
{
    if (!strcmp(_DEBUG_LEVEL, "trickle") || !strcmp(_DEBUG_LEVEL, "stream") || !strcmp(_DEBUG_LEVEL, "flood"))
    {
        va_list args;
        va_start(args, f);
        vprintf(f, args);
        va_end(args);
    }
}

/**
 * fatal
 *
 * there is a HUGE error
 * print a message
 * then give up and exit with error status `1`
 *
 * credit: https://piazza.com/class/lls99rbz11v578/post/10
 * credit: CSCI5229 example ex4
 */
    __attribute__((format(printf, 1, 2)))
void fatal(char const* f, ...)
{
    va_list args;
    va_start(args, f);
    vprintf(f, args);
    va_end(args);
    exit(1);
}

/**
 * errorCheck
 *
 * see if an error occured from oGL and tell the user
 * 
 * credit: https://piazza.com/class/lls99rbz11v578/post/10
 * credit: CSCI5229 example ex5
 */
void errorCheck(const char* where)
{
    int err = glGetError();
    if (err) printStuff("\n\n\nXXXXXXXXXX\tERROR %s:\n\t%s\n\n\n\n", gluErrorString(err), where);
    else debugFlood("error check passed %s\n", where);
}

