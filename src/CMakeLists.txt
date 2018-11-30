add_library(dlldr SHARED dlldr.cpp dlldr-dlopen.cpp)
target_link_libraries(dlldr ${CMAKE_DL_LIBS} stdc++fs)
