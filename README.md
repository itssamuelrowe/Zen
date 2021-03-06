# Zen

Zen is a general purpose programming language designed to build simple, reliable and efficient programs.

> Zen is currently under development. Please contact the author,
> Samuel Rowe (<samuelrowe1999@gmail.com>), to contribute.

Visit the [official website](https://itssamuelrowe.github.io/zen-website/) to learn
more about the language, tools, getting started, and more.

This repository contains the reference implementation of the Zen compiler.
Please explore the following links to find other relevant resources.
 * [The Zen Programming Language Specification](https://github.com/itssamuelrowe/The-Zen-Programming-Language-Specification)
 * [The Zen Virtual Machine Specification](https://github.com/itssamuelrowe/The-Zen-Virtual-Machine-Specification)
 * [The Zen Programming Language Manual](https://github.com/itssamuelrowe/The-Zen-Programming-Language-Manual)

## Example 1

```python
function factorial(n)
    return n <= 1 ? 1 : n * factorial(n - 1)

function main(...arguments)
    printf('Enter an integer: ')
    var n = scanInteger()
    var result = factorial(n)
    print('%d! = %d', n, result)
```

The above example generates the following output:

```
Enter an integer: 5
5! = 120
```

## Example 2
```python
function main(...arguments)
    printf('What is your name? ')
    var name = scanLine()
    if name.isBlank()
        print('The specified name is blank.')
    else if name == 'Samuel Rowe'
        print('Hey! That is my name, too!')
    else
        print('Hi, %s!', name)
```

The above example generates the following output:

```
What is your name? Samuel Rowe
Hey! That is my name, too!
```

## Example 3

```python
function main(...arguments)
    var names = [
        'Samuel Rowe',
        'Joel E. Rego',
        'Bill Gates',
        'Linus Trovalds',
        'Abraham Lincoln',
        'Isaac Newton',
        'Albert Einstein',
        'Richard Feynman',
        'Christopher Nolan',
        'Benedict Cumberbatch'
    ]
    var key = 'Marshall Mathers'
    var result = null
    for var i in range(0, names.size)
        if key == names[i]
            result = i
            break

    if result != null
        print('Found the key "%s" at index %d.', key, result)
    else
        print('Could not find the key "%s" in the list.', key)
```

The above example generates the following output:

```
Could not find the key "Marshall Mathers" in the list.
```

## Installation

Before you install Zen, you need to install JTK and pkg-config on your system.

JTK is a library designed for writing applications and libraries in C. It provides
core utilities such as collections, unit testing, I/O streams, threads and much
more. You can find the latest version of JTK [here](https://github.com/itssamuelrowe/jtk).

### Compiling Zen on Windows

For compiling Zen on Windows, you first need to install MSYS2 or WSL.
The steps required to install MSYS2 or WSL are beyond the scope of this
documentation. Although you can find tutorials on installing them on the
Internet.

Please follow the steps given below if you are compiling on MSYS2. If you
are using WSL to compile Zen, please follow the steps described under the
Linux section.

1. Extract the source code to any directory of your choice, for the sake of this
   documentation we will refer this location as 'C:\zen'.
2. Change the directory to 'C:\zen'.
    ```
    cd 'C:/zen'
    ```
3. Create a temporary directory for compiling the source code. For the sake of this
   documentation we will refer to this directory as 'build'.
   ```
   mkdir build
   ```
4. Change directory to the newly created directory.
   ```
   cd build
   ```
5. Invoke CMake to generate make files. In this documentation, we show the
   commands to generate GNU Makefiles. You can easily generate other makefiles
   similarly.
   ```
   cmake .. -G 'MSYS Makefiles'
   ```
6. Invoke the GNU Make program. The command may be different on your system,
   if you have not setup an alias.
   ```
   make
   ```
   This should compile the compiler, virtual machine, and all the other executables.
   Archives and executable files should be produced in your current working directory.

   As of this version, a plethora of warnings are generated. We are working to
   eradicate these warnings.

### Compiling Zen on Linux and WSL

1. For compiling Zen on Linux or WSL, you need CMake and GNU Make (or any other make
   utility that CMake is compatible with).
2. Extract the source code to any directory of your choice, for the sake of this
   documentation we will refer this location as '/mnt/g/zen'.
3. Change the directory to '/mnt/g/zen'.
    ```
    cd '/mnt/g/zen'
    ```
4. Create a temporary directory for compiling the source code. For the sake of this
   documentation we will refer to this directory as 'build'.
   ```
   mkdir build
   ```
5. Change directory to the newly created directory.
   ```
   cd build
   ```
6. Invoke CMake to generate make files. In this documentation, we show the
   commands to generate the default Makefiles, on our system GNU Makefiles is
   the default target. You can easily generate other makefiles by specifying
   the target make files using the `-G` flag.
   ```
   cmake ..
   ```
7. Invoke the GNU Make program.
   ```
   make
   ```
   This should compile the compiler, virtual machine, and all the other executables.
   Archives and executable files should be produced in your current working directory.

   As of this version, a plethora of warnings are generated. We are working to
   eradicate these warnings.

## Contributing

We welcome all contributors.

Zen was created with a vision to help programmers, like you and I, write code
better. With your contributions, we can get there sooner. We appreciate your help!

To contribute, please contact the original author Samuel Rowe (<samuelrowe1999@gmail.com>).

## License

Copyright 2017-2020 Samuel Rowe

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
