include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NAME FakeCamTCPClient)
project(${NAME} VERSION 0.1.0)

# Add source to this project's executable.
file(GLOB_RECURSE SOURCES 
${PROJECT_SOURCE_DIR}/Src/*.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})

set(ArrayPool_PATH ArrayPool/Src/)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${ArrayPool_PATH}
    )
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
      ${ArrayPool_PATH}
    )
endif()