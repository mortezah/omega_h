#ifndef OMEGA_H_BUILD_HPP
#define OMEGA_H_BUILD_HPP

#include <Omega_h_array.hpp>
#include <Omega_h_comm.hpp>

namespace Omega_h {

class Mesh;
struct Remotes;

void build_from_elems2verts(
    Mesh* mesh, CommPtr comm, Int edim, LOs ev2v, Read<GO> vert_globals);
void build_from_elems2verts(Mesh* mesh, Int edim, LOs ev2v, LO nverts);
void build_from_elems_and_coords(Mesh* mesh, Int edim, LOs ev2v, Reals coords);
void build_box(Mesh* mesh, Real x, Real y, Real z, LO nx, LO ny, LO nz);

void add_ents2verts(Mesh* mesh, Int edim, LOs ev2v, Read<GO> vert_globals);
void resolve_derived_copies(CommPtr comm, Read<GO> verts2globs, Int deg,
    LOs* p_ent_verts2verts, Remotes* p_ents2owners);

}  // end namespace Omega_h

#endif
