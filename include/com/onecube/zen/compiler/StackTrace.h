// Friday, January 05, 2018

#ifndef ZEN_STACK_TRACE_H
#define ZEN_STACK_TRACE_H

/*******************************************************************************
 * StackTrace                                                                  *
 *******************************************************************************/

void zen_StackTrace_enter0(const char* file, const char* name);
void zen_StackTrace_exit0(const char* file, const char* name);

#endif /* ZEN_STACK_TRACE_H */

#define zen_StackTrace_enter() zen_StackTrace_enter0(__FILE__, __FUNCTION__)
#define zen_StackTrace_exit() zen_StackTrace_exit0(__FILE__, __FUNCTION__)