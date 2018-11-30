add_executable(ld-local ld-local.cpp)
add_library(module-a MODULE module-a.cpp)

# TODO: It's for only libstdc++, make for others.
target_link_libraries(ld-local dlldr stdc++fs)


