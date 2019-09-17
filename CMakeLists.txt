# Friday, November 03, 2017

cmake_minimum_required (VERSION 3.4.1)

# Packages

include(FindPkgConfig)

pkg_search_module(JTK REQUIRED jtk)

# Project Name

project (Zen)

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

    # Abstract Syntax Tree

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstAnnotation.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstAnnotations.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstHelper.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstListener.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstNode.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstPrinter.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/ast/AstWalker.c

    # Generator

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/generator/BinaryEntityBuilder.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/generator/DataChannel.c

    # Lexer

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/lexer/Lexer.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/lexer/LexerError.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/lexer/Token.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/lexer/TokenStream.c

    # Parser

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/parser/Context.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/parser/Parser.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/parser/ParserError.c

    # Symbol Table

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/SymbolDefinitionListener.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/SymbolResolutionListener.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/SymbolTable.c

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/scope/ClassScope.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/scope/CompilationUnitScope.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/scope/EnumerationScope.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/scope/FunctionScope.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/scope/LocalScope.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/scope/Scope.c

    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/ClassSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/ConstantSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/EnumerateSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/EnumerationSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/FunctionSignature.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/FunctionSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/LabelSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/MemberConstantSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/MemberVariableSymbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/Symbol.c
    ${PROJECT_SOURCE_DIR}/source/com/onecube/zen/compiler/symbol-table/symbol/VariableSymbol.c
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

# Compile

set(CMAKE_VERBOSE_MAKEFILE off)

link_directories(${JTK_LIBRARY_DIRS})

message(WARNING ${JTK_CFLAGS})

add_executable (zen ${ZEN_PUBLIC_HEADERS} ${ZEN_COMMON_SOURCE} ${ZEN_COMPILER_SOURCE} ${ZEN_PRIVATE_HEADERS})
target_link_libraries(zen ${JTK_LIBRARIES})
target_include_directories(zen SYSTEM PUBLIC ${JTK_INCLUDE_DIRS})
# target_compile_options(zen PUBLIC -Wall -Wswitch)
target_compile_options(zen PUBLIC -g -w ${JTK_CFLAGS} ${JTK_CFLAGS_OTHER})



# add_executable (zsv ${ZEN_PUBLIC_HEADERS} ${ZEN_COMMON_SOURCE} ${ZEN_ZVM_SPECIFICATION_VIEWER} ${ZEN_PRIVATE_HEADERS})
# target_compile_options(zsv PUBLIC -Wall -Wswitch)


# add_executable(zvm ${ZEN_COMMON_SOURCE} ${ZEN_VIRTUAL_MACHINE_SOURCE} ${ZEN_VIRTUAL_MACHINE_TOOL_SOURCE})
# target_link_libraries(zvm ${JTK_LIBRARIES})
# target_include_directories(zvm SYSTEM PUBLIC ${JTK_INCLUDE_DIRS})
# target_compile_options(zvm PUBLIC -g -w ${JTK_CFLAGS} ${JTK_CFLAGS_OTHER})

message (WARNING ${JTK_LIBRARY_DIRS})