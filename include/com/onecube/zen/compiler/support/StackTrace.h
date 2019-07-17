// Friday, January 05, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_SUPPORT_STACK_TRACE_H
#define COM_ONECUBE_ZEN_COMPILER_SUPPORT_STACK_TRACE_H

/*******************************************************************************
 * StackTrace                                                                  *
 *******************************************************************************/

void zen_StackTrace_enter0(const char* file, const char* name);
void zen_StackTrace_exit0(const char* file, const char* name);

#define zen_StackTrace_enter() zen_StackTrace_enter0(__FILE__, __FUNCTION__)
#define zen_StackTrace_exit() zen_StackTrace_exit0(__FILE__, __FUNCTION__)

#endif /* COM_ONECUBE_ZEN_COMPILER_SUPPORT_STACK_TRACE_H */