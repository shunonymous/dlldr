add_library(dlldr SHARED dlldr.cpp)

if(UNIX)
  add_library(dlldr-platform STATIC dlldr-dlopen)
elseif(WIN32)
  add_library(dlldr-platform STATIC dlldr-w32)
endif()

target_link_libraries(dlldr ${CMAKE_DL_LIBS} stdc++fs dlldr-platform)
