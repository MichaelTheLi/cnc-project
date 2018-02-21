set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER gcc)

add_definitions("-g")
add_definitions("-DTEST_BUILD")
add_definitions("-DTEST_EXECUTOR")
add_definitions("-DF_CPU=1000000")

find_package(OpenGL)
find_package(GLUT)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

file(GLOB_RECURSE SRC_TESTS_C_FILES ${PROJECT_SOURCE_DIR}/simulator/*.c)
file(GLOB_RECURSE SRC_TESTS_HEADERS ${PROJECT_SOURCE_DIR}/simulator/*.h)

add_library(simulator_bits ../src/bits.c)
add_library(simulator_stepper_driver ../src/stepper/stepper.c)
add_library(simulator_gcode_driver ../src/gcode/parser.c)
add_library(simulator_gcode_executor ../src/gcode/executor.c)
add_library(simulator_planner_linear ../src/planner/linear.c)
add_library(simulator_planner_arc ../src/planner/arc.c)
add_library(simulator_stepper_hardware_driver ../src/stepper/driver/hardware_driver.c)
add_library(simulator_stepper_software_driver ../src/stepper/driver/software_driver.c)

include_directories(../src)

set(COMPILED_FOR_PC simulator_bits simulator_stepper_driver simulator_gcode_driver simulator_gcode_executor simulator_planner_linear simulator_planner_arc simulator_stepper_hardware_driver simulator_stepper_software_driver)

set(LIBS ${LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${COMPILED_FOR_PC})

add_executable(simulator_main ${SRC_TESTS_C_FILES} ${SRC_TESTS_HEADERS})
target_link_libraries(simulator_main ${LIBS})

set(CMAKE_VERBOSE_MAKEFILE ON)