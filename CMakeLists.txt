cmake_minimum_required(VERSION 3.10)
project(PartitionedShadowVolume C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Options
# -------

# Force the use of GLEW - if found - to handle OpenGL extensions,
# otherwise a python script will generate functions loaders.
set(USE_GLEW ON)


# Dependencies
# ------------

if(MSVC)
  set(WIN_THIRDPARTIES "C:/Libraries") # tmp

  set(GLFW_STATIC_LIBRARIES "glfw3")
  set(GLFW_INCLUDE_DIRS "${WIN_THIRDPARTIES}/include/")
  link_directories(${WIN_THIRDPARTIES}/lib64/)
else()
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
endif()

find_package(OpenGL REQUIRED)

if(USE_GLEW)
  find_package(GLEW 1.10)
  if(${GLEW_FOUND})
    add_definitions(-DUSE_GLEW)
  endif()
endif(USE_GLEW)

# Paths
# -----

set(SRCDIR "./src")
set(SHADERDIR "./shaders")
set(OBJLOADER "./objLoader")
set(COMMON "./common")
set(THIRDPARTY "./thirdparty")
set(TOOLS "./tools")
set(HEADERS "./src/headers")


# Main target build
# -----------------

set(TARGET_NAME psv)
file(GLOB Sources */*.cc)
file(GLOB Headers */*/*.h)
file(GLOB Miscs */*.glsl)

add_executable(${TARGET_NAME}
  ${Sources}
  ${Headers}
  ${Miscs}
)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CXX_FLAGS "-g -O3 -Wall -std=c++11 -lGLEW ")
elseif(MSVC)
  set(CXX_FLAGS "")
elseif(${CMAKE_CXX_COMPILER})
  message(WARNING "untested compiler")
else()
  message(FATAL_ERROR "no compiler was found")
endif()

set_target_properties(${TARGET_NAME} PROPERTIES
                      LINKER_LANGUAGE CXX)

include_directories(
  ${SRCDIR}
  ${THIRDPARTY}
  ${OBJLOADER}
  ${COMMON}
  ${HEADERS}
  ${GLFW_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_NAME}
  ${GLFW_STATIC_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}

)
