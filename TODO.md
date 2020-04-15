  * Implement a wildcard search
 * Implement virtual function tables, static function tables, instance field tables, and static field tables.
 * Static functions should not go through ZenKernel.invokeStatic()!
 * Native code should be able throw and catch exceptions.
 * Implement line numbers for exceptions.
 * Generate default constructors when no constructor is provided.
 * Move semantic errors from the code generator to definition and resolution phases.
 * Use symbol->m_name instead of symbol->m_context.as_Class.qualifiedName
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
 * Implement hash code caching for strings
 * Remove two level caching for entity loader.
 * Remove usage of Tape to improve performance.
 * Implement decimal values!
 * Implement the logical operators
 * Implement the compound assignment operators
 * Throw an exception when dividing by zero.
 * Implement continue statement
 * Change the function keyword to define. Functions should be referenced as methods.
 * Escape sequences!
 * Function should return null if none is provided
 * Function overloading
 * Call other constructors
 * Implement printf()
 * Implement @Property
 * Implicit classes created from functions declared in compilation unit cannot be instantiated.

 Python: 0.153s, Zen: 0.744s, JavaScript: 0.273s
 Python: 0.292s (2.64x faster), Zen: 0.773s, JavaScript: 0.319s (2.42x faster)
 Python: 0.043s (17.41x faster), Zen: 0.749s, JavaScript: 0.281s (2.66x faster)
 Python: 0.042s (15.19x faster), Zen: 0.638s, JavaScript: 0.274s (2.32x faster)