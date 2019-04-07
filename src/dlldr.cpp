/* dlldr.cpp - A implimentation of P0275R4, platform independation part
 * Author: Shun Terabayashi <shunonymous@gmail.com>
 * License: Boost Software License Version 1.0
 */

#include <vector>
#include <regex>

#include "dlldr.hpp"

namespace dlldr
{
    // Copy constructor
    shared_library::shared_library(shared_library&& lib) noexcept
    {
	handler = lib.handler;
    }

    shared_library::shared_library(const filesystem::path& library_path)
    {
	error_code ec;
	load_library(library_path, default_mode, ec);
    }

    shared_library::shared_library(const filesystem::path& library_path, dl_mode mode)
    {
	error_code ec;
	load_library(library_path, mode, ec);
    }

    shared_library::shared_library(const filesystem::path& library_path, error_code& ec)
    {
	load_library(library_path, default_mode, ec);
    }

    shared_library::shared_library(const filesystem::path& library_path, dl_mode mode, error_code& ec)
    {
	load_library(library_path, mode, ec);
    }

    void shared_library::reset() noexcept
    {
	unload_library();
    }

    std::vector<filesystem::path> shared_library::decoration(const filesystem::path& library_path, dl_mode& mode)
    {
	std::vector<filesystem::path> decorated_pathes;
	filesystem::path path = library_path;


	if((mode bitand search_system_directories) == search_system_directories)
	{
	    if(path.has_parent_path())
		std::cerr << "WARNING: dlldr: library_path includes directory in search_system_directories mode, strip." << std::endl;

	    // When libpath not includes "/", dlopen searches library from system directories.
	    path = path.filename();
	    mode = mode xor search_system_directories;
	} else {
	    // If libpath not includes "/", add "./" to top and to be recognized as relative libpath by dlopen.
	    if(!path.has_parent_path())
		path = filesystem::path("./") / path;
	}
	
	if((mode bitand add_decorations) == add_decorations)
	{	
	    std::regex re("(.+)");

	    for(auto&& fmt : decorate_formats)
	    {
		// /usr/lib/example -> /usr/lib/libexample.so
		decorated_pathes.push_back(
		    path.replace_filename(
			std::regex_replace(library_path.filename().string(), re, fmt)));
	    }
	    mode = mode xor add_decorations;
	} else {
	    decorated_pathes.push_back(path);
	}

	return decorated_pathes;
    }
}
