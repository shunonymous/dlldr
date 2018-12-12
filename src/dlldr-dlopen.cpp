/* dlldr-dlopen.cpp - A implimentation of P0275R4, POSIX specific part
 * Author: Shun Terabayashi <shunonymous@gmail.com>
 * License: Boost Software License Version 1.0
 */

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

#include <dlfcn.h>

#include "dlldr.hpp"

namespace dlldr
{
    void shared_library::load_library(const filesystem::path& library_path, shared_library::dl_mode mode, error_code& ec)
    {
	std::vector<filesystem::path> path_list = decoration(library_path, mode);

	if(mode == 0)
	    mode = default_mode;

	for(auto&& p : path_list)
	    if(!handler)
	    {
		handler = dlopen(p.c_str(), mode);

		if(!handler)
		{
		    ec = dlerror();
		    std::cerr << p.c_str() << " failed to load, " << ec << std::endl;
		}
		dlerror();
	    }
    }

    shared_library::~shared_library()
    {
	dlclose(handler);
    }

    shared_library::native_symbol_type shared_library::getSymAddr(const char* symbol_name) const
    {
	char* error;
	void* symaddr;
	symaddr = dlsym(handler, symbol_name);

	error = dlerror();

	if(error != nullptr)
	{
	    std::cout << error << std::endl;
	    std::exit(1);
	}
	    
	return symaddr;
    }
}
