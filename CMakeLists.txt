cmake_minimum_required(VERSION 3.10)

project(pascc C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(pascc
    main.c
    ast.c
    codegen.c
    semantic.c
    symtab.c
    lex.yy.c
    y.tab.c
)

target_include_directories(pascc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX AND EXISTS "/data/workspace/myshixun/bin")
    set_target_properties(pascc PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "/data/workspace/myshixun/bin"
    )
endif()
