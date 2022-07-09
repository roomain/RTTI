set(ProjectName "unit_tests")

file(GLOB_RECURSE SRCS "*.cpp")
file(GLOB_RECURSE INCS "*.h")

file(GLOB_RECURSE SRCS_TEST "${CMAKE_SOLUTION_DIR}/RTTI/*.cpp")
file(GLOB_RECURSE INCS_TEST "${CMAKE_SOLUTION_DIR}/RTTI/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/.. PREFIX "RTTI/header files" FILES ${INCS_TEST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/.. PREFIX "RTTI/source files" FILES ${SRCS_TEST})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "header files" FILES ${INCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source files" FILES ${SRCS})

target_include_directories(${ProjectName} PUBLIC "${CMAKE_SOLUTION_DIR}/RTTI")

add_library(${ProjectName} SHARED ${INCS_TEST} ${SRCS_TEST} ${INCS} ${SRCS})