#ifndef MIMALLOC_H
#define MIMALLOC_H

#define MI_MALLOC_VERSION 214   // major + 2 digits minor

// ------------------------------------------------------
// Compiler specific attributes
// ------------------------------------------------------

#if defined(__GNUC__)
  #if defined(MI_SHARED_LIB) && defined(MI_SHARED_LIB_EXPORT)
    #define export __attribute__((visibility("default")))
  #else
    #define export
  #endif
  #define mi_cdecl                      // leads to warnings... __attribute__((cdecl))
  #define mi_decl_restrict
  #define mi_attr_malloc                __attribute__((malloc))
  #if (defined(__clang_major__) && (__clang_major__ < 4)) || (__GNUC__ < 5)
    #define mi_attr_alloc_size(s)
    #define mi_attr_alloc_size2(s1,s2)
    #define mi_attr_alloc_align(p)
  #elif defined(__INTEL_COMPILER)
    #define mi_attr_alloc_size(s)       __attribute__((alloc_size(s)))
    #define mi_attr_alloc_size2(s1,s2)  __attribute__((alloc_size(s1,s2)))
    #define mi_attr_alloc_align(p)
  #else
    #define mi_attr_alloc_size(s)       __attribute__((alloc_size(s)))
    #define mi_attr_alloc_size2(s1,s2)  __attribute__((alloc_size(s1,s2)))
    #define mi_attr_alloc_align(p)      __attribute__((alloc_align(p)))
  #endif
#else
  #define mi_cdecl
  #define mi_decl_export
  #define mi_decl_restrict
  #define mi_attr_malloc
  #define mi_attr_alloc_size(s)
  #define mi_attr_alloc_size2(s1,s2)
  #define mi_attr_alloc_align(p)
#endif

// ------------------------------------------------------
// Includes
// ------------------------------------------------------

#include <stddef.h>     // size_t
#include <stdbool.h>    // bool
#include <stdint.h>     // INTPTR_MAX

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------------------------
// Standard malloc interface
// ------------------------------------------------------

export mi_decl_restrict void* mi_malloc(size_t size)  mi_attr_malloc mi_attr_alloc_size(1);
export mi_decl_restrict void* mi_calloc(size_t count, size_t size)  mi_attr_malloc mi_attr_alloc_size2(1,2);
export void* mi_realloc(void* p, size_t newsize)      mi_attr_alloc_size(2);
export void* mi_expand(void* p, size_t newsize)                         mi_attr_alloc_size(2);

export void mi_free(void* p);
export mi_decl_restrict char* mi_strdup(const char* s) mi_attr_malloc;
export mi_decl_restrict char* mi_strndup(const char* s, size_t n) mi_attr_malloc;
export mi_decl_restrict char* mi_realpath(const char* fname, char* resolved_name) mi_attr_malloc;

// ------------------------------------------------------
// Extended functionality
// ------------------------------------------------------
#define MI_SMALL_WSIZE_MAX  (128)
#define MI_SMALL_SIZE_MAX   (MI_SMALL_WSIZE_MAX*sizeof(void*))

export mi_decl_restrict void* mi_malloc_small(size_t size) mi_attr_malloc mi_attr_alloc_size(1);
export mi_decl_restrict void* mi_zalloc_small(size_t size) mi_attr_malloc mi_attr_alloc_size(1);
export mi_decl_restrict void* mi_zalloc(size_t size)       mi_attr_malloc mi_attr_alloc_size(1);

export mi_decl_restrict void* mi_mallocn(size_t count, size_t size) mi_attr_malloc mi_attr_alloc_size2(1,2);
export void* mi_reallocn(void* p, size_t count, size_t size)        mi_attr_alloc_size2(2,3);
export void* mi_reallocf(void* p, size_t newsize)                   mi_attr_alloc_size(2);

export size_t mi_usable_size(const void* p);
export size_t mi_good_size(size_t size)    ;


// ------------------------------------------------------
// Internals
// ------------------------------------------------------

typedef void (mi_cdecl mi_deferred_free_fun)(bool force, unsigned long long heartbeat, void* arg);
export void mi_register_deferred_free(mi_deferred_free_fun* deferred_free, void* arg);

typedef void (mi_cdecl mi_output_fun)(const char* msg, void* arg);
export void mi_register_output(mi_output_fun* out, void* arg);

typedef void (mi_cdecl mi_error_fun)(int err, void* arg);
export void mi_register_error(mi_error_fun* fun, void* arg);

export void mi_collect(bool force)   ;
export int  mi_version(void)         ;
export void mi_stats_reset(void)     ;
export void mi_stats_merge(void)     ;
export void mi_stats_print(void* out);  // backward compatibility: `out` is ignored and should be NULL
export void mi_stats_print_out(mi_output_fun* out, void* arg);

export void mi_process_init(void)    ;
export void mi_thread_init(void)     ;
export void mi_thread_done(void)     ;
export void mi_thread_stats_print_out(mi_output_fun* out, void* arg);

export void mi_process_info(size_t* elapsed_msecs, size_t* user_msecs, size_t* system_msecs,
                                    size_t* current_rss, size_t* peak_rss,
                                    size_t* current_commit, size_t* peak_commit, size_t* page_faults);

// -------------------------------------------------------------------------------------
// Aligned allocation
// Note that `alignment` always follows `size` for consistency with unaligned
// allocation, but unfortunately this differs from `posix_memalign` and `aligned_alloc`.
// -------------------------------------------------------------------------------------

export mi_decl_restrict void* mi_malloc_aligned(size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
export mi_decl_restrict void* mi_malloc_aligned_at(size_t size, size_t alignment, size_t offset) mi_attr_malloc mi_attr_alloc_size(1);
export mi_decl_restrict void* mi_zalloc_aligned(size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
export mi_decl_restrict void* mi_zalloc_aligned_at(size_t size, size_t alignment, size_t offset) mi_attr_malloc mi_attr_alloc_size(1);
export mi_decl_restrict void* mi_calloc_aligned(size_t count, size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size2(1,2) mi_attr_alloc_align(3);
export mi_decl_restrict void* mi_calloc_aligned_at(size_t count, size_t size, size_t alignment, size_t offset) mi_attr_malloc mi_attr_alloc_size2(1,2);
export void* mi_realloc_aligned(void* p, size_t newsize, size_t alignment) mi_attr_alloc_size(2) mi_attr_alloc_align(3);
export void* mi_realloc_aligned_at(void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_alloc_size(2);


// -------------------------------------------------------------------------------------
// Heaps: first-class, but can only allocate from the same thread that created it.
// -------------------------------------------------------------------------------------

struct mi_heap_s;
typedef struct mi_heap_s mi_heap_t;

export mi_heap_t* mi_heap_new(void);
export void       mi_heap_delete(mi_heap_t* heap);
export void       mi_heap_destroy(mi_heap_t* heap);
export mi_heap_t* mi_heap_set_default(mi_heap_t* heap);
export mi_heap_t* mi_heap_get_default(void);
export mi_heap_t* mi_heap_get_backing(void);
export void       mi_heap_collect(mi_heap_t* heap, bool force);

export mi_decl_restrict void* mi_heap_malloc(mi_heap_t* heap, size_t size) mi_attr_malloc mi_attr_alloc_size(2);
export mi_decl_restrict void* mi_heap_zalloc(mi_heap_t* heap, size_t size) mi_attr_malloc mi_attr_alloc_size(2);
export mi_decl_restrict void* mi_heap_calloc(mi_heap_t* heap, size_t count, size_t size) mi_attr_malloc mi_attr_alloc_size2(2, 3);
export mi_decl_restrict void* mi_heap_mallocn(mi_heap_t* heap, size_t count, size_t size) mi_attr_malloc mi_attr_alloc_size2(2, 3);
export mi_decl_restrict void* mi_heap_malloc_small(mi_heap_t* heap, size_t size) mi_attr_malloc mi_attr_alloc_size(2);

export void* mi_heap_realloc(mi_heap_t* heap, void* p, size_t newsize)              mi_attr_alloc_size(3);
export void* mi_heap_reallocn(mi_heap_t* heap, void* p, size_t count, size_t size)  mi_attr_alloc_size2(3,4);
export void* mi_heap_reallocf(mi_heap_t* heap, void* p, size_t newsize)             mi_attr_alloc_size(3);

export mi_decl_restrict char* mi_heap_strdup(mi_heap_t* heap, const char* s)            mi_attr_malloc;
export mi_decl_restrict char* mi_heap_strndup(mi_heap_t* heap, const char* s, size_t n) mi_attr_malloc;
export mi_decl_restrict char* mi_heap_realpath(mi_heap_t* heap, const char* fname, char* resolved_name) mi_attr_malloc;

export mi_decl_restrict void* mi_heap_malloc_aligned(mi_heap_t* heap, size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(3);
export mi_decl_restrict void* mi_heap_malloc_aligned_at(mi_heap_t* heap, size_t size, size_t alignment, size_t offset) mi_attr_malloc mi_attr_alloc_size(2);
export mi_decl_restrict void* mi_heap_zalloc_aligned(mi_heap_t* heap, size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(3);
export mi_decl_restrict void* mi_heap_zalloc_aligned_at(mi_heap_t* heap, size_t size, size_t alignment, size_t offset) mi_attr_malloc mi_attr_alloc_size(2);
export mi_decl_restrict void* mi_heap_calloc_aligned(mi_heap_t* heap, size_t count, size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size2(2, 3) mi_attr_alloc_align(4);
export mi_decl_restrict void* mi_heap_calloc_aligned_at(mi_heap_t* heap, size_t count, size_t size, size_t alignment, size_t offset) mi_attr_malloc mi_attr_alloc_size2(2, 3);
export void* mi_heap_realloc_aligned(mi_heap_t* heap, void* p, size_t newsize, size_t alignment) mi_attr_alloc_size(3) mi_attr_alloc_align(4);
export void* mi_heap_realloc_aligned_at(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_alloc_size(3);


// --------------------------------------------------------------------------------
// Zero initialized re-allocation.
// Only valid on memory that was originally allocated with zero initialization too.
// e.g. `mi_calloc`, `mi_zalloc`, `mi_zalloc_aligned` etc.
// see <https://github.com/microsoft/mimalloc/issues/63#issuecomment-508272992>
// --------------------------------------------------------------------------------

export void* mi_rezalloc(void* p, size_t newsize)                mi_attr_alloc_size(2);
export void* mi_recalloc(void* p, size_t newcount, size_t size)  mi_attr_alloc_size2(2,3);

export void* mi_rezalloc_aligned(void* p, size_t newsize, size_t alignment) mi_attr_alloc_size(2) mi_attr_alloc_align(3);
export void* mi_rezalloc_aligned_at(void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_alloc_size(2);
export void* mi_recalloc_aligned(void* p, size_t newcount, size_t size, size_t alignment) mi_attr_alloc_size2(2,3) mi_attr_alloc_align(4);
export void* mi_recalloc_aligned_at(void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_alloc_size2(2,3);

export void* mi_heap_rezalloc(mi_heap_t* heap, void* p, size_t newsize)                mi_attr_alloc_size(3);
export void* mi_heap_recalloc(mi_heap_t* heap, void* p, size_t newcount, size_t size)  mi_attr_alloc_size2(3,4);

export void* mi_heap_rezalloc_aligned(mi_heap_t* heap, void* p, size_t newsize, size_t alignment) mi_attr_alloc_size(3) mi_attr_alloc_align(4);
export void* mi_heap_rezalloc_aligned_at(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_alloc_size(3);
export void* mi_heap_recalloc_aligned(mi_heap_t* heap, void* p, size_t newcount, size_t size, size_t alignment) mi_attr_alloc_size2(3,4) mi_attr_alloc_align(5);
export void* mi_heap_recalloc_aligned_at(mi_heap_t* heap, void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_alloc_size2(3,4);


// ------------------------------------------------------
// Analysis
// ------------------------------------------------------

export bool mi_heap_contains_block(mi_heap_t* heap, const void* p);
export bool mi_heap_check_owned(mi_heap_t* heap, const void* p);
export bool mi_check_owned(const void* p);

// An area of heap space contains blocks of a single size.
typedef struct mi_heap_area_s {
  void*  blocks;      // start of the area containing heap blocks
  size_t reserved;    // bytes reserved for this area (virtual)
  size_t committed;   // current available bytes for this area
  size_t used;        // number of allocated blocks
  size_t block_size;  // size in bytes of each block
  size_t full_block_size; // size in bytes of a full block including padding and metadata.
} mi_heap_area_t;

typedef bool (mi_cdecl mi_block_visit_fun)(const mi_heap_t* heap, const mi_heap_area_t* area, void* block, size_t block_size, void* arg);

export bool mi_heap_visit_blocks(const mi_heap_t* heap, bool visit_all_blocks, mi_block_visit_fun* visitor, void* arg);

// Experimental
export bool mi_is_in_heap_region(const void* p);
export bool mi_is_redirected(void);

export int mi_reserve_huge_os_pages_interleave(size_t pages, size_t numa_nodes, size_t timeout_msecs);
export int mi_reserve_huge_os_pages_at(size_t pages, int numa_node, size_t timeout_msecs);

export int  mi_reserve_os_memory(size_t size, bool commit, bool allow_large);
export bool mi_manage_os_memory(void* start, size_t size, bool is_committed, bool is_large, bool is_zero, int numa_node);

export void mi_debug_show_arenas(bool show_inuse, bool show_abandoned, bool show_purge);

// Experimental: heaps associated with specific memory arena's
typedef int mi_arena_id_t;
export void* mi_arena_area(mi_arena_id_t arena_id, size_t* size);
export int   mi_reserve_huge_os_pages_at_ex(size_t pages, int numa_node, size_t timeout_msecs, bool exclusive, mi_arena_id_t* arena_id);
export int   mi_reserve_os_memory_ex(size_t size, bool commit, bool allow_large, bool exclusive, mi_arena_id_t* arena_id);
export bool  mi_manage_os_memory_ex(void* start, size_t size, bool is_committed, bool is_large, bool is_zero, int numa_node, bool exclusive, mi_arena_id_t* arena_id);

#if MI_MALLOC_VERSION >= 182
// Create a heap that only allocates in the specified arena
export mi_heap_t* mi_heap_new_in_arena(mi_arena_id_t arena_id);
#endif

// deprecated
export int  mi_reserve_huge_os_pages(size_t pages, double max_secs, size_t* pages_reserved);


// ------------------------------------------------------
// Convenience
// ------------------------------------------------------

#define mi_malloc_tp(tp)                ((tp*)mi_malloc(sizeof(tp)))
#define mi_zalloc_tp(tp)                ((tp*)mi_zalloc(sizeof(tp)))
#define mi_calloc_tp(tp,n)              ((tp*)mi_calloc(n,sizeof(tp)))
#define mi_mallocn_tp(tp,n)             ((tp*)mi_mallocn(n,sizeof(tp)))
#define mi_reallocn_tp(p,tp,n)          ((tp*)mi_reallocn(p,n,sizeof(tp)))
#define mi_recalloc_tp(p,tp,n)          ((tp*)mi_recalloc(p,n,sizeof(tp)))

#define mi_heap_malloc_tp(hp,tp)        ((tp*)mi_heap_malloc(hp,sizeof(tp)))
#define mi_heap_zalloc_tp(hp,tp)        ((tp*)mi_heap_zalloc(hp,sizeof(tp)))
#define mi_heap_calloc_tp(hp,tp,n)      ((tp*)mi_heap_calloc(hp,n,sizeof(tp)))
#define mi_heap_mallocn_tp(hp,tp,n)     ((tp*)mi_heap_mallocn(hp,n,sizeof(tp)))
#define mi_heap_reallocn_tp(hp,p,tp,n)  ((tp*)mi_heap_reallocn(hp,p,n,sizeof(tp)))
#define mi_heap_recalloc_tp(hp,p,tp,n)  ((tp*)mi_heap_recalloc(hp,p,n,sizeof(tp)))


// ------------------------------------------------------
// Options
// ------------------------------------------------------

typedef enum mi_option_e {
  // stable options
  mi_option_show_errors,                // print error messages
  mi_option_show_stats,                 // print statistics on termination
  mi_option_verbose,                    // print verbose messages
  // advanced options
  mi_option_eager_commit,               // eager commit segments? (after `eager_commit_delay` segments) (=1)
  mi_option_arena_eager_commit,         // eager commit arenas? Use 2 to enable just on overcommit systems (=2)
  mi_option_purge_decommits,            // should a memory purge decommit? (=1). Set to 0 to use memory reset on a purge (instead of decommit)
  mi_option_allow_large_os_pages,       // allow large (2 or 4 MiB) OS pages, implies eager commit. If false, also disables THP for the process.
  mi_option_reserve_huge_os_pages,      // reserve N huge OS pages (1GiB pages) at startup
  mi_option_reserve_huge_os_pages_at,   // reserve huge OS pages at a specific NUMA node
  mi_option_reserve_os_memory,          // reserve specified amount of OS memory in an arena at startup
  mi_option_deprecated_segment_cache,
  mi_option_deprecated_page_reset,
  mi_option_abandoned_page_purge,       // immediately purge delayed purges on thread termination
  mi_option_deprecated_segment_reset, 
  mi_option_eager_commit_delay,         // the first N segments per thread are not eagerly committed (but per page in the segment on demand)
  mi_option_purge_delay,                // memory purging is delayed by N milli seconds; use 0 for immediate purging or -1 for no purging at all. (=10)
  mi_option_use_numa_nodes,             // 0 = use all available numa nodes, otherwise use at most N nodes.
  mi_option_disallow_os_alloc,          // 1 = do not use OS memory for allocation (but only programmatically reserved arenas)
  mi_option_os_tag,                     // tag used for OS logging (macOS only for now) (=100)
  mi_option_max_errors,                 // issue at most N error messages
  mi_option_max_warnings,               // issue at most N warning messages
  mi_option_max_segment_reclaim,        // max. percentage of the abandoned segments can be reclaimed per try (=10%)
  mi_option_destroy_on_exit,            // if set, release all memory on exit; sometimes used for dynamic unloading but can be unsafe
  mi_option_arena_reserve,              // initial memory size in KiB for arena reservation (= 1 GiB on 64-bit)
  mi_option_arena_purge_mult,           // multiplier for `purge_delay` for the purging delay for arenas (=10)
  mi_option_purge_extend_delay,
  mi_option_abandoned_reclaim_on_free,  // allow to reclaim an abandoned segment on a free (=1)
  mi_option_disallow_arena_alloc,       // 1 = do not use arena's for allocation (except if using specific arena id's)
  _mi_option_last,
  // legacy option names
  mi_option_large_os_pages = mi_option_allow_large_os_pages,
  mi_option_eager_region_commit = mi_option_arena_eager_commit,
  mi_option_reset_decommits = mi_option_purge_decommits,
  mi_option_reset_delay = mi_option_purge_delay,
  mi_option_abandoned_page_reset = mi_option_abandoned_page_purge,
  mi_option_limit_os_alloc = mi_option_disallow_os_alloc
} mi_option_t;


export bool mi_option_is_enabled(mi_option_t option);
export void mi_option_enable(mi_option_t option);
export void mi_option_disable(mi_option_t option);
export void mi_option_set_enabled(mi_option_t option, bool enable);
export void mi_option_set_enabled_default(mi_option_t option, bool enable);

export long   mi_option_get(mi_option_t option);
export long   mi_option_get_clamp(mi_option_t option, long min, long max);
export size_t mi_option_get_size(mi_option_t option);
export void mi_option_set(mi_option_t option, long value);
export void mi_option_set_default(mi_option_t option, long value);


// -------------------------------------------------------------------------------------------------------
// "mi" prefixed implementations of various posix, Unix, Windows, and C++ allocation functions.
// (This can be convenient when providing overrides of these functions as done in `mimalloc-override.h`.)
// note: we use `mi_cfree` as "checked free" and it checks if the pointer is in our heap before free-ing.
// -------------------------------------------------------------------------------------------------------

export void  mi_cfree(void* p);
export void* mi__expand(void* p, size_t newsize);
export size_t mi_malloc_size(const void* p)       ;
export size_t mi_malloc_good_size(size_t size)    ;
export size_t mi_malloc_usable_size(const void *p);

export int mi_posix_memalign(void** p, size_t alignment, size_t size)  ;
export mi_decl_restrict void* mi_memalign(size_t alignment, size_t size) mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(1);
export mi_decl_restrict void* mi_valloc(size_t size)  mi_attr_malloc mi_attr_alloc_size(1);
export mi_decl_restrict void* mi_pvalloc(size_t size) mi_attr_malloc mi_attr_alloc_size(1);
export mi_decl_restrict void* mi_aligned_alloc(size_t alignment, size_t size) mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(1);

export void* mi_reallocarray(void* p, size_t count, size_t size) mi_attr_alloc_size2(2,3);
export int   mi_reallocarr(void* p, size_t count, size_t size);
export void* mi_aligned_recalloc(void* p, size_t newcount, size_t size, size_t alignment);
export void* mi_aligned_offset_recalloc(void* p, size_t newcount, size_t size, size_t alignment, size_t offset);

export mi_decl_restrict unsigned short* mi_wcsdup(const unsigned short* s) mi_attr_malloc;
export mi_decl_restrict unsigned char*  mi_mbsdup(const unsigned char* s)  mi_attr_malloc;
export int mi_dupenv_s(char** buf, size_t* size, const char* name)                     ;
export int mi_wdupenv_s(unsigned short** buf, size_t* size, const unsigned short* name);

export void mi_free_size(void* p, size_t size)                          ;
export void mi_free_size_aligned(void* p, size_t size, size_t alignment);
export void mi_free_aligned(void* p, size_t alignment)                  ;

// The `mi_new` wrappers implement C++ semantics on out-of-memory instead of directly returning `NULL`.
// (and call `std::get_new_handler` and potentially raise a `std::bad_alloc` exception).
export mi_decl_restrict void* mi_new(size_t size)                   mi_attr_malloc mi_attr_alloc_size(1);
export mi_decl_restrict void* mi_new_aligned(size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
export mi_decl_restrict void* mi_new_nothrow(size_t size)           mi_attr_malloc mi_attr_alloc_size(1);
export mi_decl_restrict void* mi_new_aligned_nothrow(size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
export mi_decl_restrict void* mi_new_n(size_t count, size_t size)   mi_attr_malloc mi_attr_alloc_size2(1, 2);
export void* mi_new_realloc(void* p, size_t newsize)                mi_attr_alloc_size(2);
export void* mi_new_reallocn(void* p, size_t newcount, size_t size) mi_attr_alloc_size2(2, 3);

export mi_decl_restrict void* mi_heap_alloc_new(mi_heap_t* heap, size_t size)                mi_attr_malloc mi_attr_alloc_size(2);
export mi_decl_restrict void* mi_heap_alloc_new_n(mi_heap_t* heap, size_t count, size_t size) mi_attr_malloc mi_attr_alloc_size2(2, 3);

#ifdef __cplusplus
}
#endif

// ---------------------------------------------------------------------------------------------
// Implement the C++ std::allocator interface for use in STL containers.
// (note: see `mimalloc-new-delete.h` for overriding the new/delete operators globally)
// ---------------------------------------------------------------------------------------------
#ifdef __cplusplus

#include <cstddef>     // std::size_t
#include <cstdint>     // PTRDIFF_MAX
#if (__cplusplus >= 201103L) || (_MSC_VER > 1900)  // C++11
#include <type_traits> // std::true_type
#include <utility>     // std::forward
#endif

template<class T> struct _mi_stl_allocator_common {
  typedef T                 value_type;
  typedef std::size_t       size_type;
  typedef std::ptrdiff_t    difference_type;
  typedef value_type&       reference;
  typedef value_type const& const_reference;
  typedef value_type*       pointer;
  typedef value_type const* const_pointer;

  #if ((__cplusplus >= 201103L) || (_MSC_VER > 1900))  // C++11
  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap            = std::true_type;
  template <class U, class ...Args> void construct(U* p, Args&& ...args) { ::new(p) U(std::forward<Args>(args)...); }
  template <class U> void destroy(U* p) { p->~U(); }
  #else
  void construct(pointer p, value_type const& val) { ::new(p) value_type(val); }
  void destroy(pointer p) { p->~value_type(); }
  #endif

  size_type     max_size() const { return (PTRDIFF_MAX/sizeof(value_type)); }
  pointer       address(reference x) const        { return &x; }
  const_pointer address(const_reference x) const  { return &x; }
};

template<class T> struct mi_stl_allocator : public _mi_stl_allocator_common<T> {
  using typename _mi_stl_allocator_common<T>::size_type;
  using typename _mi_stl_allocator_common<T>::value_type;
  using typename _mi_stl_allocator_common<T>::pointer;
  template <class U> struct rebind { typedef mi_stl_allocator<U> other; };

  mi_stl_allocator()                                             = default;
  mi_stl_allocator(const mi_stl_allocator&)                      = default;
  template<class U> mi_stl_allocator(const mi_stl_allocator<U>&) { }
  mi_stl_allocator  select_on_container_copy_construction() const { return *this; }
  void              deallocate(T* p, size_type) { mi_free(p); }

  #if (__cplusplus >= 201703L)  // C++17
  T* allocate(size_type count) { return static_cast<T*>(mi_new_n(count, sizeof(T))); }
  T* allocate(size_type count, const void*) { return allocate(count); }
  #else
  pointer allocate(size_type count, const void* = 0) { return static_cast<pointer>(mi_new_n(count, sizeof(value_type))); }
  #endif

  #if ((__cplusplus >= 201103L) || (_MSC_VER > 1900))  // C++11
  using is_always_equal = std::true_type;
  #endif
};

template<class T1,class T2> bool operator==(const mi_stl_allocator<T1>& , const mi_stl_allocator<T2>& ) { return true; }
template<class T1,class T2> bool operator!=(const mi_stl_allocator<T1>& , const mi_stl_allocator<T2>& ) { return false; }


#if (__cplusplus >= 201103L) || (_MSC_VER >= 1900)  // C++11
#define MI_HAS_HEAP_STL_ALLOCATOR 1

#include <memory>      // std::shared_ptr

// Common base class for STL allocators in a specific heap
template<class T, bool _mi_destroy> struct _mi_heap_stl_allocator_common : public _mi_stl_allocator_common<T> {
  using typename _mi_stl_allocator_common<T>::size_type;
  using typename _mi_stl_allocator_common<T>::value_type;
  using typename _mi_stl_allocator_common<T>::pointer;

  _mi_heap_stl_allocator_common(mi_heap_t* hp) : heap(hp, [](mi_heap_t*) {}) {}    /* will not delete nor destroy the passed in heap */

  #if (__cplusplus >= 201703L)  // C++17
  T* allocate(size_type count) { return static_cast<T*>(mi_heap_alloc_new_n(this->heap.get(), count, sizeof(T))); }
  T* allocate(size_type count, const void*) { return allocate(count); }
  #else
  pointer allocate(size_type count, const void* = 0) { return static_cast<pointer>(mi_heap_alloc_new_n(this->heap.get(), count, sizeof(value_type))); }
  #endif

  #if ((__cplusplus >= 201103L) || (_MSC_VER > 1900))  // C++11
  using is_always_equal = std::false_type;
  #endif

  void collect(bool force) { mi_heap_collect(this->heap.get(), force); }
  template<class U> bool is_equal(const _mi_heap_stl_allocator_common<U, _mi_destroy>& x) const { return (this->heap == x.heap); }

protected:
  std::shared_ptr<mi_heap_t> heap;
  template<class U, bool D> friend struct _mi_heap_stl_allocator_common;

  _mi_heap_stl_allocator_common() {
    mi_heap_t* hp = mi_heap_new();
    this->heap.reset(hp, (_mi_destroy ? &heap_destroy : &heap_delete));  /* calls heap_delete/destroy when the refcount drops to zero */
  }
  _mi_heap_stl_allocator_common(const _mi_heap_stl_allocator_common& x) : heap(x.heap) { }
  template<class U> _mi_heap_stl_allocator_common(const _mi_heap_stl_allocator_common<U, _mi_destroy>& x) : heap(x.heap) { }

private:
  static void heap_delete(mi_heap_t* hp)  { if (hp != NULL) { mi_heap_delete(hp); } }
  static void heap_destroy(mi_heap_t* hp) { if (hp != NULL) { mi_heap_destroy(hp); } }
};

// STL allocator allocation in a specific heap
template<class T> struct mi_heap_stl_allocator : public _mi_heap_stl_allocator_common<T, false> {
  using typename _mi_heap_stl_allocator_common<T, false>::size_type;
  mi_heap_stl_allocator() : _mi_heap_stl_allocator_common<T, false>() { } // creates fresh heap that is deleted when the destructor is called
  mi_heap_stl_allocator(mi_heap_t* hp) : _mi_heap_stl_allocator_common<T, false>(hp) { }  // no delete nor destroy on the passed in heap
  template<class U> mi_heap_stl_allocator(const mi_heap_stl_allocator<U>& x) : _mi_heap_stl_allocator_common<T, false>(x) { }

  mi_heap_stl_allocator select_on_container_copy_construction() const { return *this; }
  void deallocate(T* p, size_type) { mi_free(p); }
  template<class U> struct rebind { typedef mi_heap_stl_allocator<U> other; };
};

template<class T1, class T2> bool operator==(const mi_heap_stl_allocator<T1>& x, const mi_heap_stl_allocator<T2>& y) { return (x.is_equal(y)); }
template<class T1, class T2> bool operator!=(const mi_heap_stl_allocator<T1>& x, const mi_heap_stl_allocator<T2>& y) { return (!x.is_equal(y)); }


// STL allocator allocation in a specific heap, where `free` does nothing and
// the heap is destroyed in one go on destruction -- use with care!
template<class T> struct mi_heap_destroy_stl_allocator : public _mi_heap_stl_allocator_common<T, true> {
  using typename _mi_heap_stl_allocator_common<T, true>::size_type;
  mi_heap_destroy_stl_allocator() : _mi_heap_stl_allocator_common<T, true>() { } // creates fresh heap that is destroyed when the destructor is called
  mi_heap_destroy_stl_allocator(mi_heap_t* hp) : _mi_heap_stl_allocator_common<T, true>(hp) { }  // no delete nor destroy on the passed in heap
  template<class U> mi_heap_destroy_stl_allocator(const mi_heap_destroy_stl_allocator<U>& x) : _mi_heap_stl_allocator_common<T, true>(x) { }

  mi_heap_destroy_stl_allocator select_on_container_copy_construction() const { return *this; }
  void deallocate(T*, size_type) { /* do nothing as we destroy the heap on destruct. */ }
  template<class U> struct rebind { typedef mi_heap_destroy_stl_allocator<U> other; };
};

template<class T1, class T2> bool operator==(const mi_heap_destroy_stl_allocator<T1>& x, const mi_heap_destroy_stl_allocator<T2>& y) { return (x.is_equal(y)); }
template<class T1, class T2> bool operator!=(const mi_heap_destroy_stl_allocator<T1>& x, const mi_heap_destroy_stl_allocator<T2>& y) { return (!x.is_equal(y)); }

#endif // C++11

#endif // __cplusplus

#endif
