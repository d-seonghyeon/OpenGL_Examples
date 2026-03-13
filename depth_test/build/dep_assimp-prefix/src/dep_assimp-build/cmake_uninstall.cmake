IF(NOT EXISTS "/Users/SH/Desktop/Computer_Graphics/OpenGL/depth_test/build/dep_assimp-prefix/src/dep_assimp-build/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"/Users/SH/Desktop/Computer_Graphics/OpenGL/depth_test/build/dep_assimp-prefix/src/dep_assimp-build/install_manifest.txt\"")
ENDIF(NOT EXISTS "/Users/SH/Desktop/Computer_Graphics/OpenGL/depth_test/build/dep_assimp-prefix/src/dep_assimp-build/install_manifest.txt")

FILE(READ "/Users/SH/Desktop/Computer_Graphics/OpenGL/depth_test/build/dep_assimp-prefix/src/dep_assimp-build/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "/opt/homebrew/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
