cmake_minimum_required(VERSION 3.8)

project("ArbeitSoftware" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Sources, Forms and Resources ////////////////////////////////////////////////

add_executable("${PROJECT_NAME}"
    "src/ArbeitSoftware.cpp"
    "src/ArbeitSoftware.h"
)

# Compiler Options ////////////////////////////////////////////////////////////

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /MP /GS /W3 /Zc:wchar_t /Gm- /Od /Zc:inline /fp:precise /WX- /Zc:forScope /RTC1 /Gd /MDd /FC /EHsc
    )
endif()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /MP /GS /W3 /Zc:wchar_t /Gm- /O2 /Zc:inline /fp:precise /GT /WX- /Zc:forScope /Gd /Oi /MD /FC /EHsc /Ot
    )
endif()

