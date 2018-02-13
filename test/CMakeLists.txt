enable_testing()
find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
#include_directories("/usr/local/Cellar/avr-gcc/7.2.0/avr/include")

#include_directories("/usr/local/Cellar/simavr/HEAD-6660aba/include/simavr")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER gcc)

add_definitions("-g")
add_definitions("-DLOGGED_BITS")
add_definitions("-DTEST_BUILD")
add_definitions("-DTEST_EXECUTOR")
add_definitions("-DF_CPU=1000000")

find_package(OpenGL)
find_package(GLUT)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

######################
# Compiling libraries for PC here
######################
#file(GLOB_RECURSE SRC_C_FILES_FOR_TESTS ${PROJECT_SOURCE_DIR}/src/*.c)
#file(GLOB_RECURSE SRC_H_FILES_FOR_TESTS ${PROJECT_SOURCE_DIR}/src/*.h)
add_library(test_bits ../src/bits.c)
add_library(test_planner_linear ../src/planner/linear.c)
add_library(test_planner_arc ../src/planner/arc.c)
add_library(test_stepper_software_driver ../src/stepper/driver/software_driver.c)
add_library(test_stepper_hardware_driver ../src/stepper/driver/hardware_driver.c)
add_library(test_stepper_driver ../src/stepper/stepper.c)
add_library(test_pwm_calculator ../src/servo/pwm_calculator.c)
add_library(test_servo_driver ../src/servo/driver.c)
add_library(test_gcode_driver ../src/gcode/parser.c)
add_library(test_gcode_executor ../src/gcode/executor.c)
#add_library(test_src_lib ${SRC_C_FILES_FOR_TESTS} ${SRC_H_FILES_FOR_TESTS})

set(COMPILED_FOR_PC test_pwm_calculator test_bits test_planner_linear test_planner_arc test_stepper_software_driver test_stepper_hardware_driver test_servo_driver test_gcode_driver test_gcode_executor test_stepper_driver)

set(LIBS ${LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CHECK_LIBRARIES} ${COMPILED_FOR_PC})
#set(LIBS ${LIBS} ${CHECK_LIBRARIES} test_src_lib)

file(GLOB_RECURSE SRC_TESTS_C_FILES ${PROJECT_SOURCE_DIR}/test/*.c)
file(GLOB_RECURSE SRC_TESTS_HEADERS ${PROJECT_SOURCE_DIR}/test/*.h)

include_directories(../src)

add_executable(test_main ${SRC_TESTS_C_FILES} ${SRC_TESTS_HEADERS})
target_link_libraries(test_main ${LIBS})

add_test(test_main ${CMAKE_CURRENT_BINARY_DIR}/test_main)
