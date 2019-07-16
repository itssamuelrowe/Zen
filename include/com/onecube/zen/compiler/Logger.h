// Sunday, December 03, 2017

#ifndef ZEN_LOGGER_H
#define ZEN_LOGGER_H

#include <com/onecube/zen/LogPriority.h>

/*******************************************************************************
 * Logger                                                                      *
 *******************************************************************************/

void zen_Logger_log(zen_LogPriority_t priority, const char* message, ...);
 
#endif /* ZEN_LOGGER_H */