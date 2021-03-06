#ifndef OMEGA_H_KOKKOS_HPP
#define OMEGA_H_KOKKOS_HPP

#include <Omega_h_c.h>
#include <string>

#ifdef OMEGA_H_USE_KOKKOSCORE

OMEGA_H_SYSTEM_HEADER

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <Kokkos_Core.hpp>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#if defined(KOKKOS_HAVE_CUDA) && !defined(OMEGA_H_USE_CUDA)
#error "Kokkos has CUDA, please reconfigure with Omega_h_USE_CUDA=ON"
#endif

#endif  // OMEGA_H_USE_KOKKOSCORE

#ifdef OMEGA_H_USE_KOKKOSCORE
#define OMEGA_H_INLINE KOKKOS_INLINE_FUNCTION
#else
#define OMEGA_H_INLINE inline
#endif  // OMEGA_H_USE_KOKKOSCORE

#ifdef OMEGA_H_USE_CUDA
#define OMEGA_H_DEVICE __device__ inline
#define OMEGA_H_LAMBDA [=] __device__
#else
#define OMEGA_H_DEVICE inline
#define OMEGA_H_LAMBDA [=]
#endif  // OMEGA_H_USE_CUDA

#ifdef OMEGA_H_USE_CUDA
#define OMEGA_H_CONSTANT_DATA __constant__
#else
#define OMEGA_H_CONSTANT_DATA
#endif

namespace Omega_h {

inline void begin_code(std::string const& name) {
#ifdef OMEGA_H_USE_KOKKOSCORE
  Kokkos::Profiling::pushRegion(name);
#else
  (void)name;
#endif
}

inline void end_code() {
#ifdef OMEGA_H_USE_KOKKOSCORE
  Kokkos::Profiling::popRegion();
#endif
}

}  // namespace Omega_h

#endif
