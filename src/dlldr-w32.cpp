/* dlldr-w32.cpp - A implimentation of P0275R4, Win32API specific part
 * Author: Shun Terabayashi <shunonymous@gmail.com>
 * License: Boost Software License Version 1.0
 */

#define UNICODE

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
	libpath = library_path;

	// TODO: Extend "lib"
	if((mode bitand add_decorations) == add_decorations)
	{
//	    libpath += Extension;
	    mode = mode xor add_decorations;
	}
	
	if((mode bitand search_system_directories) == search_system_directories)
	{
//	    if(libpath.has_parent_path())
//		std::cerr << "WARNING: dlldr: library_libpath includes directory in search_system_directories mode, strip." << std::endl;

	    // When libpath not includes "/", dlopen searches library from system directories.
//	    libpath = libpath.filename();
	    mode = mode xor search_system_directories;
	} else {
	    // If libpath not includes "/", add "./" to top and to be recognized as relative libpath by dlopen.
//	    if(!libpath.has_parent_path())
//		libpath = filesystem::path(".\\") / libpath;
	}

	handler = LoadLibrary(libpath.c_str());
    }

    shared_library::~shared_library()
    {
	FreeLibrary(handler);
    }

    shared_library::native_symbol_type shared_library::getSymAddr(const char* symbol_name) const
    {
	FARPROC symaddr = GetProcAddress(handler, symbol_name);
	return symaddr;
    }
}
