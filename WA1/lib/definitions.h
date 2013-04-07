#ifndef LIB_DEFINITIONS_H
#define LIB_DEFINITIONS_H

/**
 * debug info */
#define DEBUG false


/**
 * Image size and location */
static const double WIDTH = 2048;
static const double HEIGHT = 2048;
static const double X_MIN = -1;
static const double Y_MIN = -1;
static const double X_MAX = 1;
static const double Y_MAX = 1;


int debug_info(const char *format, ...);

#endif
