#add_library(dlldr SHARED dlldr.cpp)

if(UNIX)
  add_library(dlldr SHARED dlldr.cpp)
  add_library(dlldr-platform STATIC dlldr-dlopen.cpp)
elseif(WIN32)
  # TODO: make shared library on Windows(mingw-w64)
  add_library(dlldr STATIC dlldr.cpp)
  add_library(dlldr-platform STATIC dlldr-w32.cpp)
endif()

target_link_libraries(dlldr ${CMAKE_DL_LIBS} ${CXXFS_LIBRARY} dlldr-platform)
