 * Implement the import statement
 * Implement default import strategy.
 * Implement a wildcard search
 * Implement virtual function tables, static function tables, instance field tables, and static field tables.
 * Static functions should not go through ZenKernel.invokeStatic()!
 * Native code should be able throw and catch exceptions.
 * Implement line numbers for exceptions.
 * Generate FEB for compilation units with only functions.
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