enable_testing()
find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

#include_directories("/usr/local/Cellar/simavr/HEAD-6660aba/include/simavr")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER gcc)

add_definitions("-g")

######################
# Compiling libraries for PC here
######################
add_library(test_stepper_software_driver ../src/stepper/software_driver.c)
add_library(test_gcode_driver ../src/gcode/parser.c)



set(LIBS ${LIBS} ${CHECK_LIBRARIES} test_stepper_software_driver test_gcode_driver)

file(GLOB_RECURSE SRC_TESTS_C_FILES ${PROJECT_SOURCE_DIR}/test/*.c)
file(GLOB_RECURSE SRC_TESTS_HEADERS ${PROJECT_SOURCE_DIR}/test/*.h)

include_directories(../src)

add_executable(test_main ${SRC_TESTS_C_FILES} ${SRC_TESTS_HEADERS})
target_link_libraries(test_main ${LIBS})

add_test(test_main ${CMAKE_CURRENT_BINARY_DIR}/test_main)
