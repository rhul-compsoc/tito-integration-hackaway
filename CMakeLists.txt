cmake_minimum_required(VERSION 3.12)

# set the project name
project(tito-integegration-hackaway)
set(REPO_URL "https://github.com/rhul-compsoc/tito-integegration-hackaway")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# compiler flags
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wno-unused-parameter -Wno-writable-strings -DDEBUG=1 -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS_RELEASE "-O2 -Wno-unused-parameter -Wno-writable-strings -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)

if (CMAKE_BUILD_TYPE STREQUAL "TEST")
    include(CodeCoverage)
    
    # Setup coverage (using gcovr so needs gcc/g++)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
    
    append_coverage_compiler_flags()
    #setup_target_for_coverage_gcovr_html
    setup_target_for_coverage_gcovr_xml(NAME coverage
        EXECUTABLE tito-integegration-hackaway-tests
        EXCLUDE "build/*" "tests/*" "src/main.cpp")

endif()

# src
set(MAIN
    src/main.cpp
    )
set(SRC_FILES
    src/tito.cpp
    )
    
add_executable(tito-integegration-hackaway ${SRC_FILES} ${MAIN})

target_include_directories(tito-integegration-hackaway PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "TEST")
    # tests, set the flags manually
    set(CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_DEBUG})
    set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_DEBUG})

    set (TEST_FILES
        ${SRC_FILES}
        tests/runner.cpp
        tests/testTito.cpp
        )
    add_executable(tito-integegration-hackaway-tests ${TEST_FILES})
    target_link_libraries(tito-integegration-hackaway-tests
        cppunit)
    target_include_directories(tito-integegration-hackaway-tests PUBLIC 
        ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME tito-integegration-hackaway-tests COMMAND tito-integegration-hackaway-tests)
endif()

