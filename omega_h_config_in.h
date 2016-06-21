#ifndef OMEGA_H_CONFIG_H
#define OMEGA_H_CONFIG_H

#define OSH_MAJOR @OSH_MAJOR@
#define OSH_MINOR @OSH_MINOR@
#define OSH_PATCH @OSH_PATCH@

#cmakedefine OSH_USE_MPI
#cmakedefine OSH_USE_KOKKOS
#cmakedefine OSH_USE_OPENMP
#cmakedefine OSH_USE_CUDA
#cmakedefine OSH_USE_ZLIB
#cmakedefine OSH_CHECK_BOUNDS

/* this block of preprocessor code creates a string
   literal describing the version and compile options
   used to configure this header.
   this is used to ensure that during user application
   compiles, the header included matches the library linked to.
   it is not foolproof, but better than nothing */
#define OSH_TOSTR2(s) #s
#define OSH_TOSTR(s) OSH_TOSTR2(s)
#ifdef OSH_USE_MPI
#define OSH_MPI_STR "1"
#else
#define OSH_MPI_STR "0"
#endif
#ifdef OSH_USE_KOKKOS
#define OSH_KOKKOS_STR "1"
#else
#define OSH_KOKKOS_STR "0"
#endif
#ifdef OSH_USE_OPENMP
#define OSH_OPENMP_STR "1"
#else
#define OSH_OPENMP_STR "0"
#endif
#ifdef OSH_USE_CUDA
#define OSH_CUDA_STR "1"
#else
#define OSH_CUDA_STR "0"
#endif
#ifdef OSH_USE_ZLIB
#define OSH_ZLIB_STR "1"
#else
#define OSH_ZLIB_STR "0"
#endif
#ifdef OSH_CHECK_BOUNDS
#define OSH_BOUNDS_STR "1"
#else
#define OSH_BOUNDS_STR "0"
#endif
#define OSH_VERSION \
OSH_TOSTR(OSH_MAJOR) "." OSH_TOSTR(OSH_MINOR) "." OSH_TOSTR(OSH_PATCH) \
"+" OSH_MPI_STR OSH_KOKKOS_STR OSH_OPENMP_STR OSH_CUDA_STR OSH_ZLIB_STR \
OSH_BOUNDS_STR

#endif