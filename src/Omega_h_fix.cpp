#include <Omega_h_adapt.hpp>
#include <Omega_h_file.hpp>
#include <Omega_h_loop.hpp>
#include <Omega_h_mesh.hpp>
#include <Omega_h_metric.hpp>

#ifdef OMEGA_H_USE_EGADS
#include <Omega_h_egads.hpp>
#endif

#include <cstdio>
#include <iostream>

namespace Omega_h {

template <Int dim>
static void compute_ill_metric_dim(Mesh* mesh, AdaptOpts const& opts) {
  mesh->remove_tag(VERT, "metric");
  std::cerr << "getting element implied metrics\n";
  auto elem_metrics = get_element_implied_length_metrics(mesh);
  std::cerr << "projecting them\n";
  auto metrics = project_metrics(mesh, elem_metrics);
  mesh->add_tag(VERT, "metric", symm_ncomps(mesh->dim()), metrics);
  std::cerr << "computing qualities\n";
  mesh->ask_qualities();
  std::cerr << "writing predicted.vtu\n";
  vtk::write_vtu("predicted.vtu", mesh);
  auto prelim_quals = mesh->get_array<Real>(mesh->dim(), "quality");
  auto min_qual = opts.min_quality_allowed;
  std::cerr << "removing low-quality contributions\n";
  auto elem_metrics_w = deep_copy(elem_metrics);
  auto f = OMEGA_H_LAMBDA(LO e) {
    if (prelim_quals[e] < min_qual) {
      set_symm(elem_metrics_w, e, zero_matrix<dim, dim>());
    }
  };
  parallel_for(mesh->nelems(), f, "ignore_low_quality_implied");
  elem_metrics = Reals(elem_metrics_w);
  std::cerr << "projecting metrics elem -> node\n";
  /*auto*/ metrics = project_metrics(mesh, elem_metrics);
  for (Int i = 0; i < 5; ++i) {
    std::cerr << "metric smoothing iteration " << i << '\n';
    metrics = smooth_metric_once(mesh, metrics, true);
  }
  std::cerr << "limiting metric gradation\n";
  metrics = limit_metric_gradation(mesh, metrics, 1.0);
  mesh->remove_tag(VERT, "metric");
  mesh->add_tag(VERT, "metric",
      divide_no_remainder(metrics.size(), mesh->nverts()), metrics);
  mesh->ask_qualities();
  std::cerr << "writing corrected.vtu\n";
  vtk::write_vtu("corrected.vtu", mesh);
  std::cerr << "done with \"ill\" metric\n";
}

static void compute_ill_metric(Mesh* mesh, AdaptOpts const& opts) {
  if (mesh->dim() == 3)
    compute_ill_metric_dim<3>(mesh, opts);
  else if (mesh->dim() == 2)
    compute_ill_metric_dim<2>(mesh, opts);
  else if (mesh->dim() == 1)
    compute_ill_metric_dim<1>(mesh, opts);
  else
    OMEGA_H_NORETURN();
}

static void compute_typical_metric(Mesh* mesh, Omega_h_Isotropy isotropy) {
  auto metrics = get_implied_metrics(mesh);
  metrics = apply_isotropy(mesh->nverts(), metrics, isotropy);
  metrics = limit_metric_gradation(mesh, metrics, 1.0);
  auto ncomps = divide_no_remainder(metrics.size(), mesh->nverts());
  mesh->remove_tag(VERT, "metric");
  mesh->add_tag(VERT, "metric", ncomps, metrics);
}

static void fix_for_given_metric(
    Mesh* mesh, AdaptOpts const& adapt_opts, bool verbose) {
  if (verbose) std::cout << "computing minimum quality\n";
  auto minqual = mesh->min_quality();
  auto maxlen = mesh->max_length();
  if (verbose) std::cout << "minimum quality " << minqual << '\n';
  if (verbose) std::cout << "maximum length " << maxlen << '\n';
  AdaptOpts opts = adapt_opts;
  while (true) {
    auto minqual_old = minqual;
    opts.min_quality_allowed = minqual;
    opts.max_length_allowed = max2(maxlen, opts.max_length_desired * 2.0);
    std::cout << "max_length_allowed(" << opts.max_length_allowed << ") = max("
              << "maxlen(" << maxlen << "), max_length_desired*2("
              << opts.max_length_desired * 2.0 << "))\n";
    opts.verbosity = EXTRA_STATS;
    opts.nsliver_layers = 10;
    opts.min_quality_desired = min2(minqual + 0.1, 1.0);
#ifdef OMEGA_H_USE_EGADS
    opts.allow_snap_failure = true;
#endif
    adapt(mesh, opts);
    minqual = mesh->min_quality();
    maxlen = mesh->max_length();
    if (verbose) std::cout << "minimum quality " << minqual << '\n';
    if (verbose) std::cout << "maximum length " << maxlen << '\n';
    if (minqual < minqual_old + 1e-3) break;  // stalled
  }
}

void fix(Mesh* mesh, AdaptOpts const& adapt_opts, Omega_h_Isotropy isotropy,
    bool verbose) {
  verbose = verbose && can_print(mesh);
  if (verbose) std::cout << "Computing bad-mesh \"implied\" metric\n";
  compute_ill_metric(mesh, adapt_opts);
  fix_for_given_metric(mesh, adapt_opts, verbose);
  // TODO: commenting this out because metric quality corrections assume
  // isotropy!!! we need to fix this, but for now just skip this step
  if ((0)) {
    if (verbose) std::cout << "Computing more typical \"implied\" metric\n";
    compute_typical_metric(mesh, isotropy);
    fix_for_given_metric(mesh, adapt_opts, verbose);
  }
}

}  // namespace Omega_h
