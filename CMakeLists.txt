# Friday, November 03, 2017

cmake_minimum_required (VERSION 3.4.1)

# Project Name

project (Zen)

# Packages

include(FindPkgConfig)

pkg_search_module(JTK REQUIRED jtk)

# Project Version Number

set (ZEN_PROJECT_VERSION_MAJOR 1)
set (ZEN_PROJECT_VERSION_MINOR 0)
set (ZEN_PROJECT_VERSION_PATCH 0)
set (ZEN_PROJECT_VERSION_TWEAK 0)
set (ZEN_PROJECT_VERSION "${ZEN_PROJECT_VERSION_MAJOR}.${ZEN_PROJECT_VERSION_MINOR}.${ZEN_PROJECT_VERSION_PATCH}.${ZEN_PROJECT_VERSION_TWEAK}")

# Options

option (BUILD_examples "Build the examples for ZEN." ON)
option (BUILD_shared "Build shared libraries instead of static libraries." ON)
option (BUILD_tests "Build the unit tests for ZEN." ON)

if (NOT BUILD_TYPE)
    message (WARNING "Build type not specified, falling back to 'Release' mode. To specify build type use: -DCMAKE_BUILD_TYPE=<mode> where <mode> is Debug or Release.")
    set (BUILD_TYPE "Release" CACHE STRING "The type of build: Debug or Release." FORCE)
endif (NOT BUILD_TYPE)

# Source

include_directories ("${PROJECT_SOURCE_DIR}/include")

set (ZEN_PUBLIC_HEADERS)
set (
    ZEN_PRIVATE_HEADERS
)

set (
    ZEN_COMMON_SOURCE

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/Configuration.c
)

set (
    ZEN_COMPILER_SOURCE

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/Compiler.c

    # Abstract Syntax Tree

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstAnnotation.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstAnnotations.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstHelper.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstListener.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstNode.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstPrinter.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstWalker.c

    # Generator

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/generator/BinaryEntityGenerator.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/generator/BinaryEntityBuilder.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/generator/ConstantPoolBuilder.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/generator/DataChannel.c

    # Lexer

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/lexer/Lexer.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/lexer/LexerError.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/lexer/Token.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/lexer/TokenStream.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/lexer/TokenType.c

    # Parser

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/parser/Context.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/parser/Parser.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/parser/ParserError.c

    # Symbol Table

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/SymbolDefinitionListener.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/SymbolResolutionListener.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/SymbolTable.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/SymbolLoader.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/Modifier.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/scope/Scope.c

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/ClassSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/FunctionSignature.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/FunctionSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/Symbol.c

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/support/Error.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/support/ErrorHandler.c
)


set (
    ZEN_VIRTUAL_MACHINE_SOURCE

    # Binary Entity Format

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/Instruction.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/ConstantPool.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/Entity.c

    # Loader

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/loader/BinaryEntityParser.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/loader/ClassLoader.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/loader/EntityLoader.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/loader/AttributeParseRules.c
    #${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/loader/BinaryEntityVerifier.c

    # Memory

#    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/memory/allocator/FirstFitAllocator.c
#    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/memory/allocator/SegregatedFitsAllocator.c

    # Object

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/object/Class.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/object/Field.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/object/Function.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/object/NativeFunction.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/object/Object.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/object/ObjectArray.c

    # Processor

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/processor/Interpreter.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/processor/InvocationStack.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/processor/OperandStack.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/processor/LocalVariableArray.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/processor/StackFrame.c

    # Common

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/VirtualMachine.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/ExceptionManager.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/VirtualMachineConfiguration.c
    # ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/VirtualMachineGarbageCollectorListener.c
)

set (
    ZEN_COMPILER_TOOL_SOURCE

    # Instruction.c is from the virtual machine module. Remove references to it.
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/Instruction.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/FieldEntity.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/FunctionEntity.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/attribute/InstructionAttribute.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/Zen.c
)

set (
    ZEN_COMPILER_TOOL_SOURCE

    # Instruction.c is from the virtual machine module. Remove references to it.
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/Instruction.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/FieldEntity.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/FunctionEntity.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/attribute/InstructionAttribute.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/disassembler/BinaryEntityDisassembler.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/Zen.c
)

set (
    ZEN_ZVM_SPECIFICATION_VIEWER

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/ZvmSpecificationViewer.c
)

set (
    ZEN_VIRTUAL_MACHINE_TOOL_SOURCE

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/Zvm.c
)


set (
    ZEN_DISASSEMBLER_SOURCE

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/virtual-machine/feb/Instruction.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/disassembler/BinaryEntityDisassembler.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/DisassemblerTool.c
)

# Compile

set(CMAKE_VERBOSE_MAKEFILE off)

link_directories(${JTK_LIBRARY_DIRS})

message(WARNING ${JTK_CFLAGS})

add_executable(zc
    ${ZEN_PUBLIC_HEADERS}
    ${ZEN_COMMON_SOURCE}
    ${ZEN_COMPILER_SOURCE}
    ${ZEN_COMPILER_TOOL_SOURCE}
    ${ZEN_PRIVATE_HEADERS}
    ${ZEN_VIRTUAL_MACHINE_SOURCE}
    ${ZEN_VIRTUAL_MACHINE_TOOL_SOURCE})
target_link_libraries(zc ${JTK_LIBRARIES} m)
target_include_directories(zc SYSTEM PUBLIC ${JTK_INCLUDE_DIRS})
# target_compile_options(zc PUBLIC -Wall -Wswitch)
target_compile_options(zc PUBLIC -DZVM_MAIN_DISABLE -g -w ${JTK_CFLAGS} ${JTK_CFLAGS_OTHER})

# add_executable (zsv ${ZEN_PUBLIC_HEADERS} ${ZEN_COMMON_SOURCE} ${ZEN_ZVM_SPECIFICATION_VIEWER} ${ZEN_PRIVATE_HEADERS})
# target_compile_options(zsv PUBLIC -Wall -Wswitch)

add_executable(zvm ${ZEN_COMMON_SOURCE} ${ZEN_VIRTUAL_MACHINE_SOURCE} ${ZEN_VIRTUAL_MACHINE_TOOL_SOURCE})
target_link_libraries(zvm ${JTK_LIBRARIES} -lm)
target_include_directories(zvm SYSTEM PUBLIC ${JTK_INCLUDE_DIRS})
target_compile_options(zvm PUBLIC -O3 -DNDEBUG -DJTK_LOGGER_DISABLE -w ${JTK_CFLAGS} ${JTK_CFLAGS_OTHER})
#target_compile_options(zvm PUBLIC -g -w ${JTK_CFLAGS} ${JTK_CFLAGS_OTHER})
# target_compile_options(zvm PUBLIC -g -Wall ${JTK_CFLAGS} ${JTK_CFLAGS_OTHER})

add_executable(zd ${ZEN_COMMON_SOURCE} ${ZEN_DISASSEMBLER_SOURCE})
target_link_libraries(zd ${JTK_LIBRARIES} -lm)
target_include_directories(zd SYSTEM PUBLIC ${JTK_INCLUDE_DIRS})
target_compile_options(zd PUBLIC -O3 -DNDEBUG -DJTK_LOGGER_DISABLE -w ${JTK_CFLAGS} ${JTK_CFLAGS_OTHER})
#target_compile_options(zd PUBLIC -g -w ${JTK_CFLAGS} ${JTK_CFLAGS_OTHER})
# target_compile_options(zd PUBLIC -g -Wall ${JTK_CFLAGS} ${JTK_CFLAGS_OTHER})