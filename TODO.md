 * Implement the import statement
 * Implement default import strategy.
 * Implement a wildcard search
 * Implement virtual function tables, static function tables, instance field tables, and static field tables.
 * Static functions should not go through ZenKernel.invokeStatic()!
 * Native code should be able throw and catch exceptions.
 * Implement line numbers for exceptions.
 * Generate default constructors when no constructor is provided.
 * Move semantic errors from the code generator to definition and resolution phases.
 * The compiler generates error for
```
class Example
    ...

class Main
    function main(...arguments)
        var Example
```

But does not generate error for
```
class Main
    function main(...arguments)
        var Example

class Example
    ...
```




 * Change invoke function pointers for native functions to accept variable arguments.
 * Change zen_Object_t from structure to uint8_t*
 * Implement instance methods.
 * Implement integers