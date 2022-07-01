cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(DrogonTPY)

if(NOT FPY_INCLUDE AND NOT FPY_LIB)
    message("Python path doesn't given.")
    message("Looking for Python libraries...")
    find_package(Python COMPONENTS Interpreter Development)
    if(NOT ${Python_FOUND})
        message("Python libraries couldn't found, compiling without Embedded Python")
    else()
        message("Python libraries found! Setting them up...")
        list(APPEND BNFT_INCLUDE_DIRS ${Python_INCLUDE_DIRS})
    endif()
endif()

list(APPEND INCLUDE_FILE_LIST ${FTHIRD_PARTY_INCLUDE})


#include_directories(${INCLUDE_FILE_LIST})
message(${INCLUDE_FILE_LIST})

foreach(libName IN LISTS LIB_FILE_LIST)
    list(APPEND LIB_FILE_COMPLETE "${libName}")
    message(${libName})
endforeach()

message(${LIB_FILE_COMPLETE})

add_subdirectory(src)
add_executable(DrogonTPY main.cpp)

#target_include_directories(XE_ENV_LIBRARY PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_compile_definitions(DrogonTPY PUBLIC FPYTHON_SET)
target_include_directories(DrogonTPY PUBLIC ${INCLUDE_FILE_LIST} "${PROJECT_SOURCE_DIR}/include" ${BNFT_INCLUDE_DIRS} ${Python_INCLUDE_DIRS})
target_link_libraries(DrogonTPY XE_ENV_LIBRARY ${LIB_FILE_COMPLETE} ${Python_LIBRARIES})

message("Python_FOUND:${Python_FOUND}")
message("Python_VERSION:${Python_VERSION}")
message("Python_Development_FOUND:${Python_Development_FOUND}")
message("Python_LIBRARIES:${Python_LIBRARIES}")
message("Python_LIBRARIES:${Python_LIBRARY_DIRS}")
message("Python_INCLUDE:${Python_INCLUDE_DIRS}")

