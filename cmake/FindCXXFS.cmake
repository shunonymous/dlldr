# FindModule The C++ filesystem library. 
#
# CXXFS_FOUND: Set as true when library found
# CXXFS_LIBRARY: C++ filesystem library to be link
#
# Author: Shun Terabayashi <shunonymous@gmail.com>
# Licensed under New-BSD Licens.
#

# Select implement if not set
if(NOT (CXXFS_USE_LIBSTDCXX OR CXXFS_USE_LIBCXX))
  message("NOTE: If link was failed, plese try to set CXXFS_USE_LIBSTDCXX or CXXFS_USE_LIBCXX manually")
  if((CMAKE_CXX_COMPILER_ID MATCHES "Clang") AND APPLE)
    set(CXXFS_USE_LIBCXX true)
  elseif(MSVC)
  else()
    set(CXXFS_USE_LIBSTDCXX true)
  endif()
endif()

# Try to find library
if(CXXFS_USE_LIBSTDCXX)
  find_library(CXXFS_LIBRARY stdc++fs)
elseif(CXXFS_USE_LIBCXX)
  find_library(CXXFS_LIBRARY c++fs)
endif()

if(CXXFS_LIBRARY)
  set(CXXFS_FOUND true)
endif()
