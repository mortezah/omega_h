void swap3d_qualities(Mesh* mesh, LOs cands2edges,
    Reals* cand_quals, Read<I8>* cand_configs);
Few<LOs, 4> swap3d_keys_to_prods(Mesh* mesh, LOs keys2edges);
Few<LOs, 4> swap3d_topology(Mesh* mesh,
    LOs keys2edges,
    Read<I8> edge_configs,
    Few<LOs, 4> keys2prods);