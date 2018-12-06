/* dlldr.hpp
 * Author: Shun Terabayashi <shunonymous@gmail.com>
 * Referenced http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0275r3.html
 * License: Boost Software License Version 1.0
 */

#ifndef DLLDR_HPP
#define DLLDR_HPP

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

// Workaround for some environment has not std::filesystem
#ifdef USE_EXPERIMENTAL_FILESYSTEM
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#elif USE_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
namespace filesystem = boost::filesystem;
#else
#include <filesystem>
namespace filesystem =  std::filesystem;
#endif // USE_EXPERIMENTAL_FILESYSTEM

// On POSIX systems, include dlfcn.h heaer.
// Other systems, set RTLD_FOOBAR to 0.
// This macro is used on dl mode, and non-POSIX systems,
// these values are 0. (P0275R3 IV.4.1-2)
#ifdef _POSIX_C_SOURCE
#include <dlfcn.h>
#else
#define RTLD_LAZY 0
#define RTLD_NOW 0
#define RTLD_LOCAL 0
#define RTLD_GLOBAL 0
#endif // _POSIX_C_SOURCE


#include <string>

namespace dlldr
{
//#if __unix
    using error_code = char*;
//#endif
    // class to work with shared library files
    class shared_library;

    bool operator==(const shared_library& lhs, const shared_library& rhs) noexcept;
    bool operator!=(const shared_library& lhs, const shared_library& rhs) noexcept;
    bool operator< (const shared_library& lhs, const shared_library& rhs) noexcept;
    bool operator> (const shared_library& lhs, const shared_library& rhs) noexcept;
    bool operator<=(const shared_library& lhs, const shared_library& rhs) noexcept;
    bool operator>=(const shared_library& lhs, const shared_library& rhs) noexcept;

    // free functions
    template<class T>
    filesystem::path symbol_location(const T& symbol, error_code& ec);
    template<class T>
    filesystem::path symbol_location(const T& symbol);

    filesystem::path this_line_location(error_code& ec);
    filesystem::path this_line_location();

    filesystem::path program_location(error_code& ec);
    filesystem::path program_location();



    // support
    template <class T> struct hash;
    template <> struct hash<shared_library>;


    class shared_library {
    private:
#if __unix || !(__apple)
	std::string Extension = ".so";
#elif __apple
	std::string Extension = ".dylib";
#elif _WIN32
	std::string Extension = ".dll";
#endif
	filesystem::path libpath;
    public:
#if __unix
	using native_handle_type = void*;
	using native_symbol_type = void*;
#elif _WIN32
	using native_handle_type = HMODULE;
	using native_symbol_type = FARPROC;
#endif

	// 31.3.1.1 dll_mode operators [dll.dll_mode.operators]
	// shared library file load modes
	using dl_mode = uint16_t;
	static constexpr dl_mode rtld_lazy = RTLD_LAZY;
	static constexpr dl_mode rtld_now = RTLD_NOW;
	static constexpr dl_mode rtld_global = RTLD_GLOBAL;
	static constexpr dl_mode rtld_local = RTLD_LOCAL;
	static constexpr dl_mode default_mode = RTLD_LAZY or RTLD_LOCAL;
	static constexpr dl_mode add_decorations           = 0b1000'0000'0000'0000;
	static constexpr dl_mode search_system_directories = 0b0100'0000'0000'0000;

	// construct/copy/destruct
	shared_library() noexcept {}
	shared_library(shared_library&& lib) noexcept;
      
	explicit shared_library(const filesystem::path& library_path);
	shared_library(const filesystem::path& library_path, dl_mode mode);
	shared_library(const filesystem::path& library_path, error_code& ec);
	shared_library(const filesystem::path& library_path, dl_mode mode, error_code& ec);
	~shared_library();

    private:
	native_handle_type handler;
	void load_library(const filesystem::path& library_path, shared_library::dl_mode mode, error_code& ec);
	native_symbol_type getSymAddr(const char* symbol_name);
	void decoretion(std::vector<filesystem::path>& path_list);
    public:
	// public member functions
	shared_library& operator=(shared_library&& lib) noexcept;
	void reset() noexcept;
	explicit operator bool() const noexcept;

	template <typename SymbolT>
	SymbolT* get_if(const char* symbol_name) noexcept {
	    return reinterpret_cast<SymbolT*>(getSymAddr(symbol_name));
	}

	template <typename SymbolT>
	SymbolT* get_if(const std::string& symbol_name) const noexcept { return get_if<SymbolT>(symbol_name.c_str()); }

	template <typename SymbolT>
	SymbolT& get(const char* symbol_name) const;

	template <typename SymbolT>
	SymbolT& get(const std::string& symbol_name) const;

//	native_handle_type native_handle() const noexcept;

	// public static member functions
	static constexpr bool platform_supports_dl() noexcept;
	static constexpr bool platform_supports_dl_of_program() noexcept;
    };

}

#endif // DLLDR_HPP
