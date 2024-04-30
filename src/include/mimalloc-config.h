/**
 * mimalloc/config.h
 *
 * Local configuration options when building mimalloc in-tree.
 */

#ifndef CONFIG_H
#define CONFIG_H

/**
 * MI_SECURE [default: 0]
 * Use full security mitigations (like guard pages, allocation randomization,
 * double-free mitigation, and free-list corruption detection.
 */
#ifndef MI_SECURE
#define MI_SECURE 0
#endif

/**
 * MI_DEBUG_FULL [default: 0]
 * Use full internal heap invariant checking in DEBUG mode (expensive).
 */
#ifndef MI_DEBUG_FULL
#define MI_DEBUG_FULL 0
#endif

/**
 * MI_PADDING [default: 0]
 * Enable padding to detect heap block overflow (always on in DEBUG
 * or SECURE mode, or with Valgrind/ASAN).
 */
#ifndef MI_PADDING
#define MI_PADDING 0
#endif

/**
option(MI_OVERRIDE          "Override the standard malloc interface (e.g. define entry points for malloc() etc)" ON)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_XMALLOC           "Enable abort() call on memory allocation failure by default" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_SHOW_ERRORS       "Show error and warning messages by default (only enabled by default in DEBUG mode)" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_TRACK_VALGRIND    "Compile with Valgrind support (adds a small overhead)" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_TRACK_ASAN        "Compile with address sanitizer support (adds a small overhead)" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_TRACK_ETW         "Compile with Windows event tracing (ETW) support (adds a small overhead)" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_USE_CXX           "Use the C++ compiler to compile the library (instead of the C compiler)" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_SEE_ASM           "Generate assembly files" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_OSX_INTERPOSE     "Use interpose to override standard malloc on macOS" ON)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_OSX_ZONE          "Use malloc zone to override standard malloc on macOS" ON)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_WIN_REDIRECT      "Use redirection module ('mimalloc-redirect') on Windows if compiling mimalloc as a DLL" ON)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_LOCAL_DYNAMIC_TLS "Use slightly slower, dlopen-compatible TLS mechanism (Unix)" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_LIBC_MUSL         "Set this when linking with musl libc" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_BUILD_SHARED      "Build shared library" ON)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_BUILD_STATIC      "Build static library" ON)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_BUILD_OBJECT      "Build object library" ON)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_BUILD_TESTS       "Build test executables" ON)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_DEBUG_TSAN        "Build with thread sanitizer (needs clang)" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_DEBUG_UBSAN       "Build with undefined-behavior sanitizer (needs clang++)" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_SKIP_COLLECT_ON_EXIT "Skip collecting memory on program exit" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_NO_PADDING        "Force no use of padding even in DEBUG mode etc." OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_INSTALL_TOPLEVEL  "Install directly into $CMAKE_INSTALL_PREFIX instead of PREFIX/lib/mimalloc-version" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

option(MI_NO_THP            "Disable transparent huge pages support on Linux/Android for the mimalloc process only" OFF)
#ifndef MI_SECURE
/* #undef MI_SECURE is not set */
#endif

