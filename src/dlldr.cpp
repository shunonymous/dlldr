/* dlldr.cpp - A implimentation of P0275R3, platform independation part
 * Author: Shun Terabayashi <shunonymous@gmail.com>
 * License: Boost Software License Version 1.0
 */

#include "dlldr.hpp"

namespace dlldr
{
    // Copy constructor
    shared_library::shared_library(shared_library&& lib) noexcept
    {

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

}
