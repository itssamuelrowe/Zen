 * Finalize things on operator associativity and ZenKernel.evaluate()
 * When I declare and use a single local variable, the virtual machine crashes.
   However, declaring two local variables fixes this problem. :/
 * Add support for 
   `static native function invokeStatic(entity, name)`
   Otherwise, static functions which do not accept arguments will not be invoked!