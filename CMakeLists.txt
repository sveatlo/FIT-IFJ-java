cmake_minimum_required(VERSION 3.6)
project(proj1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/callframe.c
        src/callframe.h
        src/callstack.c
        src/context.c
        src/context.h
        src/error.c
        src/error.h
        src/expression.c
        src/expression.h
        src/ial.c
        src/ial.h
        src/inbuilt.c
        src/inbuilt.h
        src/instruction.c
        src/instruction.h
        src/interpret.c
        src/interpret.h
        src/list.c
        src/list.h
        src/main.c
        src/parser.c
        src/parser.h
        src/precedence_table.c
        src/precedence_table.h
        src/scanner.c
        src/scanner.h
        src/scanner_token.c
        src/scanner_token.h
        src/stack.c
        src/stack.h
        src/string.c
        src/string.h
        src/symbol.c
        src/symbol.h
        src/variable.h)

add_executable(proj1 ${SOURCE_FILES})