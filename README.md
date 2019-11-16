# PulsarVM

PulsarVM is the reference implementation of the Zen Virtual Machine (ZVM).

Please explore the following links to find other relevant resources.
 * [Zen](https://github.com/itssamuelrowe/Zen)
 * [The Zen Programming Language Specification](https://github.com/itssamuelrowe/The-Zen-Programming-Language-Specification)
 * [The Zen Virtual Machine Specification](https://github.com/itssamuelrowe/The-Zen-Virtual-Machine-Specification)
 * [The Zen Programming Language Manual](https://github.com/itssamuelrowe/The-Zen-Programming-Language-Manual)

## Installation

Before you install PulsarVM, you need to install JTK, Parallax and pkg-config on
your system.

JTK is a library designed for writing applications and libraries in C. It provides
core utilities such as collections, unit testing, I/O streams, threads and much
more. You can find the latest version of JTK [here](https://github.com/itssamuelrowe/JTK).

Parallax is a collection of cross platform components for building reliable,
high performance language runtime environments based on Eclipse OMR™.
You can find the latest version of Parallax [here](https://github.com/itssamuelrowe/Parallax).

### Compiling PulsarVM on Windows

For compiling PulsarVM on Windows, you first need to install MSYS2 or WSL.
The steps required to install MSYS2 or WSL are beyond the scope of this
documentation. Although you can find tutorials on installing them on the
Internet.

Please follow the steps given below if you are compiling on MSYS2. If you
are using WSL to compile Zen, please follow the steps described under the
Linux section.

1. Extract the source code to any directory of your choice, for the sake of this
   documentation we will refer this location as 'C:\pulsar'.
2. Change the directory to 'C:\pulsar'.
    ```
    cd 'C:/pulsar'
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
   This should compile the virtual machine, and all the other executables.
   Archives and executable files should be produced in your current working
   directory.

   As of this version, a plethora of warnings are generated. We are working to
   eradicate these warnings.

### Compiling PulsarVM on Linux and WSL

1. For compiling PulsarVM on Linux or WSL, you need CMake and GNU Make (or any other make
   utility that CMake is compatible with).
2. Extract the source code to any directory of your choice, for the sake of this
   documentation we will refer this location as '/mnt/g/pulsar'.
3. Change the directory to '/mnt/g/pulsar'.
    ```
    cd '/mnt/g/pulsar'
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

Copyright 2017-2019 Samuel Rowe

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
