/* dlldr.hpp
 * Author: Shun Terabayashi <shunonymous@gmail.com>
 * Referenced http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0275r4.html
 * License: Boost Software License Version 1.0
 */

#ifndef DLLDR_HPP
#define DLLDR_HPP

#include <iostream>

#ifdef _WIN32
#include <windows.h>
//#define UNICODE
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

#include <string_view>
#include <array>
#include <vector>

namespace dlldr
{
//#if __unix
    using error_code = char*;
//#endif
    // IV.4, shared_library
    class shared_library;

    // IV.5, runtime path functions
    template<class T>
    filesystem::path symbol_location(const T& symbol, error_code& ec);
    template<class T>
    filesystem::path symbol_location(const T& symbol);

    filesystem::path this_line_location(error_code& ec);
    filesystem::path this_line_location();

    filesystem::path program_location(error_code& ec);
    filesystem::path program_location();

    // IV.6, hash support
    template <class T> struct hash;
    template <> struct hash<shared_library>;

    class shared_library {
    private:
#if defined (__unix) && !defined (__apple) && !defined (_WIN32)
	static constexpr std::array<std::string_view, 1> decorate_formats = {"lib$1.so"};
#elif defined (__apple)
	static constexpr std::array<std::string_view, 2> decorate_formats = {"lib$1.dylib", "lib$1.so"};
#elif defined (_WIN32)
	static constexpr std::array<std::string_view, 2> decorate_formats = {"$1.dll", "lib$1.dll"};
#endif
    public:
#if defined (__unix) && !defined (_WIN32)
	using native_handle_type = void*;
	using native_symbol_type = void*;
#elif defined (_WIN32)
	using native_handle_type = HMODULE;
	using native_symbol_type = FARPROC;
#endif

	// shared library file load modes
	using dl_mode = uint16_t;

	// IV.4.2, dl_mode constants
	static constexpr dl_mode rtld_lazy = RTLD_LAZY;
	static constexpr dl_mode rtld_now = RTLD_NOW;
	static constexpr dl_mode rtld_global = RTLD_GLOBAL;
	static constexpr dl_mode rtld_local = RTLD_LOCAL;
	static constexpr dl_mode default_mode = RTLD_LAZY or RTLD_LOCAL;
	static constexpr dl_mode add_decorations           = 0b1000'0000'0000'0000;
	static constexpr dl_mode search_system_directories = 0b0100'0000'0000'0000;

	// IV.4.3, construct/copy/destruct
	constexpr shared_library() noexcept{}
	~shared_library() { reset(); }

	explicit shared_library(const filesystem::path& library_path);
	shared_library(const filesystem::path& library_path, dl_mode mode);
	shared_library(const filesystem::path& library_path, error_code& ec);
	shared_library(const filesystem::path& library_path, dl_mode mode, error_code& ec);

    private:
	native_handle_type handler = nullptr;
	void load_library(const filesystem::path& library_path, shared_library::dl_mode mode, error_code& ec);
	void unload_library() noexcept;
	native_symbol_type getSymAddr(const char* symbol_name) const;
	std::vector<filesystem::path> decoration(const filesystem::path& library_path, dl_mode& mode);
    public:
	// IV.4.4, public member functions
	void reset() noexcept;
	explicit operator bool() const noexcept;

	template <typename SymbolT>
	SymbolT* get_if(const char* symbol_name) const {
	    return reinterpret_cast<SymbolT*>(getSymAddr(symbol_name));
	}

	template <typename SymbolT>
	SymbolT* get_if(const std::string& symbol_name) const { return get_if<SymbolT>(symbol_name.c_str()); }

	template <typename SymbolT>
	SymbolT& get(const char* symbol_name) const;

	template <typename SymbolT>
	SymbolT& get(const std::string& symbol_name) const;

	native_handle_type native_handle() const noexcept;

//	strong_ordering operator<=>(const shared_library& other) const noexcept;

	// IV.4.5, public static member functions
	static constexpr bool platform_supports_dl() noexcept;
	static constexpr bool platform_supports_dl_of_program() noexcept;
    };

}

#endif // DLLDR_HPP
