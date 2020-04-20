 * Break down binary entity generator to smaller functions.
 * Move semantic errors from the code generator to definition and resolution phases.
 * Native code should be able throw and catch exceptions.
 * Generate default constructors when no constructor is provided.
 * Hexadecimal integer literals are not working
 * Use symbol->m_name instead of symbol->m_context.as_Class.qualifiedName
 * Implement decimal values!
 * Implement the logical operators
 * Implement the array function.

 * Implement virtual function tables
 * Implement instance field tables
 * Implement static field tables.
 * Function should return null if none is provided
 * Throw an exception when dividing by zero.
 * Implement standard packages
 * Add tableIndex to field constant pool entry.




 * Implement a wildcard search
 * Implement line numbers for exceptions.
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
 * Implement the compound assignment operators
 * Implement continue statement
 * Change the function keyword to define. Functions should be referenced as methods.
 * Escape sequences!
 * Function overloading
 * Call other constructors
 * Implement printf()
 * Implement @Property
 * Implicit classes created from functions declared in compilation unit cannot be instantiated.
 * Throw exception when private members are accessed
 * Wide mode should be enabled when load_cpr is used for indexes > 255

 Python: 0.153s, Zen: 0.744s, JavaScript: 0.273s
 Python: 0.292s (2.64x faster), Zen: 0.773s, JavaScript: 0.319s (2.42x faster)
 Python: 0.043s (17.41x faster), Zen: 0.749s, JavaScript: 0.281s (2.66x faster)
 Python: 0.042s (15.19x faster), Zen: 0.638s, JavaScript: 0.274s (2.32x faster)
 Python: 0.045s (15.11x faster), Zen: 0.680s, JavaScript: 0.315s (2.15x faster)

            0.244, 0.271