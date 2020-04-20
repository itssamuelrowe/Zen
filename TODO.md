April 20

 * Break down binary entity generator to smaller functions.
 * Move log messages from generator to builder

April 21
 * Move semantic errors from the code generator to definition and resolution phases.

April 22
 * invoke_dynamic

April 23
 * Native code should be able throw and catch exceptions.
 * Generate default constructors when no constructor is provided.

April 24
 * Hexadecimal integer literals are not working
* Implement decimal values!

April 25
 * Use symbol->m_name instead of symbol->m_context.as_Class.qualifiedName
 * Implement the logical operators
 * Implement the array function.

April 26
 * Write examples!

 * Implement virtual function tables
 * Implement instance field tables
 * Implement static field tables.
 * Function should return null if none is provided
 * Throw an exception when dividing by zero.
 * Implement standard packages
 * Add tableIndex to field constant pool entry.
 * Conditional operator


 * The code generator generates the instruction attribute for native functions, too!
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
 * Unicode escape sequences
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