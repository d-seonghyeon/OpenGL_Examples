# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix/src/dep_glfw")
  file(MAKE_DIRECTORY "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix/src/dep_glfw")
endif()
file(MAKE_DIRECTORY
  "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix/src/dep_glfw-build"
  "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix"
  "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix/tmp"
  "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix/src/dep_glfw-stamp"
  "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix/src"
  "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix/src/dep_glfw-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix/src/dep_glfw-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/SH/Desktop/Computer_Graphics/OpenGL/glsl/build/dep_glfw-prefix/src/dep_glfw-stamp${cfgdir}") # cfgdir has leading slash
endif()
