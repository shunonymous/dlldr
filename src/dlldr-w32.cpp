/* dlldr-w32.cpp - A implimentation of P0275R4, Win32API specific part
 * Author: Shun Terabayashi <shunonymous@gmail.com>
 * License: Boost Software License Version 1.0
 */

//#define UNICODE

#include <windows.h>

#ifdef USE_EXPERIMENTAL_FILESYSTEM
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#elif USE_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
namespace filesystem = boost::filesystem;
#else
#include <filesystem>
namespace filesystem =  std::filesystem;
#endif

#include <iostream>
#include <string>

#include "dlldr.hpp"

namespace dlldr
{
    void shared_library::load_library(const filesystem::path& library_path, shared_library::dl_mode mode, error_code& ec)
    {
	std::vector<filesystem::path> path_list = decoration(library_path, mode);

	for(auto&& p : path_list)
	    if(!handler)
		handler = LoadLibraryW(p.native().c_str());
    }

    void shared_library::unload_library() noexcept
    {
	FreeLibrary(handler);
    }

    shared_library::native_symbol_type shared_library::getSymAddr(const char* symbol_name) const
    {
	FARPROC symaddr = GetProcAddress(handler, symbol_name);
	return symaddr;
    }
}
