 * Implement the import statement
 * Implement default import strategy.
 * Extend the compiler to generate output files to corresponding to directories.
 * Implement a wildcard search
 * Implement virtual function tables, static function tables, instance field tables, and static field tables.
 * Static functions should not go through ZenKernel.invokeStatic()!
 * Fix memory leaks in the compiler.
 * The pop instruction should be generated for expression statements that are not assignments.
 * The errors should be reported to the error handler.
 * Native code should be able throw and catch exceptions.
 * Implement line numbers for exceptions.
 * Generate default constructors when no constructor is provided.
 * The compiler crashes for `new Example() = 1`
 * Implement a recovery strategy

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

 * Destory the tokens created by the lexer.