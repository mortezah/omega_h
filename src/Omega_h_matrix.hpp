#ifndef OMEGA_H_MATRIX_HPP
#define OMEGA_H_MATRIX_HPP

#include <Omega_h_array.hpp>
#include <Omega_h_vector.hpp>

namespace Omega_h {

/* column-first storage and indexing !!! */
template <Int m, Int n>
class Matrix : public Few<Vector<m>, n> {
 public:
  OMEGA_H_INLINE Matrix() {}
  /* these constructors accept the matrix in
   * row-first order for convenience.
   */
  inline Matrix(std::initializer_list<Vector<m>> l) : Few<Vector<m>, n>(l) {}
  inline Matrix(std::initializer_list<Real> l);
  OMEGA_H_INLINE void operator=(Matrix<m, n> const& rhs) volatile {
    Few<Vector<m>, n>::operator=(rhs);
  }
  OMEGA_H_INLINE Matrix(Matrix<m, n> const& rhs) : Few<Vector<m>, n>(rhs) {}
  OMEGA_H_INLINE Matrix(const volatile Matrix<m, n>& rhs)
      : Few<Vector<m>, n>(rhs) {}
#ifdef OMEGA_H_CHECK_BOUNDS
#define OMEGA_H_MATRIX_AT                                                      \
  OMEGA_H_CHECK(0 <= i);                                                       \
  OMEGA_H_CHECK(i < m);                                                        \
  OMEGA_H_CHECK(0 <= j);                                                       \
  OMEGA_H_CHECK(j < n);                                                        \
  return Few<Vector<m>, n>::operator[](j)[i]
#else
#define OMEGA_H_MATRIX_AT \
  return Few<Vector<m>, n>::operator[](j)[i]
#endif
  OMEGA_H_INLINE Real& operator()(Int i, Int j) {
    OMEGA_H_MATRIX_AT;
  }
  OMEGA_H_INLINE Real const& operator()(Int i, Int j) const {
    OMEGA_H_MATRIX_AT;
  }
  OMEGA_H_INLINE Real volatile& operator()(Int i, Int j) volatile {
    OMEGA_H_MATRIX_AT;
  }
  OMEGA_H_INLINE Real const volatile& operator()(Int i, Int j) const volatile {
    OMEGA_H_MATRIX_AT;
  }
#undef OMEGA_H_MATRIX_AT
};

template <Int m, Int n>
inline Matrix<m, n>::Matrix(std::initializer_list<Real> l) {
  Int k = 0;
  for (Real v : l) {
    (*this)[k % n][k / n] = v;
    ++k;
  }
}

template <Int m, Int n>
OMEGA_H_INLINE Vector<m> operator*(Matrix<m, n> a, Vector<n> b) {
  Vector<m> c = a[0] * b[0];
  for (Int j = 1; j < n; ++j) c = c + a[j] * b[j];
  return c;
}

template <Int m, Int n, Int p>
OMEGA_H_INLINE Matrix<m, n> operator*(Matrix<m, p> a, Matrix<p, n> b) {
  Matrix<m, n> c;
  for (Int j = 0; j < n; ++j) c[j] = a * b[j];
  return c;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<n, m> transpose(Matrix<m, n> a) {
  Matrix<n, m> b;
  for (Int i = 0; i < m; ++i)
    for (Int j = 0; j < n; ++j) b[i][j] = a[j][i];
  return b;
}

template <Int max_m, Int max_n>
OMEGA_H_INLINE Matrix<max_m, max_n> identity_matrix(Int m, Int n) {
  Matrix<max_m, max_n> a;
  for (Int j = 0; j < n; ++j)
    for (Int i = 0; i < m; ++i) a[j][i] = (i == j);
  return a;
}

template <Int max_m, Int max_n>
OMEGA_H_INLINE Matrix<max_m, max_n> identity_matrix() {
  return identity_matrix<max_m, max_n>(max_m, max_n);
}

OMEGA_H_INLINE Matrix<1, 1> matrix_1x1(Real a) {
  Matrix<1, 1> o;
  o[0][0] = a;
  return o;
}

OMEGA_H_INLINE Matrix<2, 2> matrix_2x2(Real a, Real b, Real c, Real d) {
  Matrix<2, 2> o;
  o[0] = vector_2(a, c);
  o[1] = vector_2(b, d);
  return o;
}

OMEGA_H_INLINE Matrix<3, 3> matrix_3x3(
    Real a, Real b, Real c, Real d, Real e, Real f, Real g, Real h, Real i) {
  Matrix<3, 3> o;
  o[0] = vector_3(a, d, g);
  o[1] = vector_3(b, e, h);
  o[2] = vector_3(c, f, i);
  return o;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<m, n> operator*(Matrix<m, n> a, Real b) {
  Matrix<m, n> c;
  for (Int j = 0; j < n; ++j) c[j] = a[j] * b;
  return c;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<m, n>& operator*=(Matrix<m, n>& a, Real b) {
  a = a * b;
  return a;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<m, n> operator*(Real a, Matrix<m, n> b) {
  return b * a;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<m, n> operator/(Matrix<m, n> a, Real b) {
  Matrix<m, n> c;
  for (Int j = 0; j < n; ++j) c[j] = a[j] / b;
  return c;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<m, n>& operator/=(Matrix<m, n>& a, Real b) {
  a = a / b;
  return a;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<m, n> operator+(Matrix<m, n> a, Matrix<m, n> b) {
  Matrix<m, n> c;
  for (Int j = 0; j < n; ++j) c[j] = a[j] + b[j];
  return c;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<m, n> operator-(Matrix<m, n> a, Matrix<m, n> b) {
  Matrix<m, n> c;
  for (Int j = 0; j < n; ++j) c[j] = a[j] - b[j];
  return c;
}

template <Int m, Int n>
OMEGA_H_INLINE Real max_norm(Matrix<m, n> a) {
  Real x = 0.0;
  for (Int j = 0; j < n; ++j)
    for (Int i = 0; i < m; ++i) x = max2(x, fabs(a[j][i]));
  return x;
}

template <Int max_m, Int max_n>
OMEGA_H_INLINE Real frobenius_norm(Int m, Int n, Matrix<max_m, max_n> a) {
  Real x = 0.0;
  for (Int j = 0; j < n; ++j)
    for (Int i = 0; i < m; ++i) x += square(a[j][i]);
  return sqrt(x);
}

template <Int m, Int n>
OMEGA_H_INLINE bool are_close(
    Matrix<m, n> a, Matrix<m, n> b, Real tol = EPSILON, Real floor = EPSILON) {
  for (Int j = 0; j < n; ++j)
    if (!are_close(a[j], b[j], tol, floor)) return false;
  return true;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<m, n> outer_product(Vector<m> a, Vector<n> b) {
  Matrix<m, n> c;
  for (Int j = 0; j < n; ++j)
    for (Int i = 0; i < m; ++i) c[j][i] = a[i] * b[j];
  return c;
}

template <Int m>
OMEGA_H_INLINE Real trace(Matrix<m, m> a) __attribute__((pure));
template <Int m>
OMEGA_H_INLINE Real trace(Matrix<m, m> a) {
  Real t = a[0][0];
  for (Int i = 1; i < m; ++i) t += a[i][i];
  return t;
}

template <Int m>
OMEGA_H_INLINE Vector<m> diagonal(Matrix<m, m> a) {
  Vector<m> v;
  for (Int i = 0; i < m; ++i) v[i] = a[i][i];
  return v;
}

template <Int m, Int n>
OMEGA_H_INLINE Matrix<m, n> zero_matrix() {
  Matrix<m, n> a;
  for (Int j = 0; j < n; ++j) a[j] = zero_vector<m>();
  return a;
}

OMEGA_H_INLINE Real determinant(Matrix<1, 1> m) { return m[0][0]; }

OMEGA_H_INLINE Real determinant(Matrix<2, 2> m) {
  Real a = m[0][0];
  Real b = m[1][0];
  Real c = m[0][1];
  Real d = m[1][1];
  return a * d - b * c;
}

OMEGA_H_INLINE Real determinant(Matrix<3, 3> m) {
  Real a = m[0][0];
  Real b = m[1][0];
  Real c = m[2][0];
  Real d = m[0][1];
  Real e = m[1][1];
  Real f = m[2][1];
  Real g = m[0][2];
  Real h = m[1][2];
  Real i = m[2][2];
  return (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i) -
         (a * f * h);
}

template <Int m, Int n>
OMEGA_H_INLINE Real inner_product(Matrix<m, n> a) {
  Real out = 0.0;
  for (Int j = 0; j < n; ++j) {
    for (Int i = 0; i < m; ++i) {
      out += square(a[j][i]);
    }
  }
  return out;
}

template <Int m, Int n>
OMEGA_H_INLINE Real norm(Matrix<m, n> a) {
  return sqrt(inner_product(a));
}

OMEGA_H_INLINE Matrix<3, 3> cross(Vector<3> a) {
  return matrix_3x3(0, -a[2], a[1], a[2], 0, -a[0], -a[1], a[0], 0);
}

OMEGA_H_INLINE Vector<3> uncross(Matrix<3, 3> c) {
  return vector_3(c[1][2] - c[2][1], c[2][0] - c[0][2], c[0][1] - c[1][0]) / 2.;
}

OMEGA_H_INLINE Matrix<1, 1> invert(Matrix<1, 1> m) {
  return matrix_1x1(1.0 / m[0][0]);
}

OMEGA_H_INLINE Matrix<2, 2> invert(Matrix<2, 2> m) {
  Real a = m[0][0];
  Real b = m[1][0];
  Real c = m[0][1];
  Real d = m[1][1];
  return matrix_2x2(d, -b, -c, a) / determinant(m);
}

OMEGA_H_INLINE Matrix<3, 3> invert(Matrix<3, 3> a) {
  Matrix<3, 3> b;
  b[0] = cross(a[1], a[2]);
  b[1] = cross(a[2], a[0]);
  b[2] = cross(a[0], a[1]);
  return transpose(b) / determinant(a);
}

template <Int m, Int n>
OMEGA_H_INLINE typename std::enable_if<(n < m), Matrix<n, m>>::type
pseudo_invert(Matrix<m, n> a) {
  auto at = transpose(a);
  return invert(at * a) * at;
}

template <Int m, Int n>
OMEGA_H_INLINE typename std::enable_if<(n > m), Matrix<n, m>>::type
pseudo_invert(Matrix<m, n> a) {
  auto at = transpose(a);
  return at * invert(a * at);
}

template <Int m>
OMEGA_H_INLINE Matrix<m, m> diagonal(Vector<m> v) {
  Matrix<m, m> a;
  for (Int i = 0; i < m; ++i)
    for (Int j = i + 1; j < m; ++j) a[i][j] = a[j][i] = 0.0;
  for (Int i = 0; i < m; ++i) a[i][i] = v[i];
  return a;
}

OMEGA_H_INLINE constexpr Int symm_ncomps(Int dim) {
  return ((dim + 1) * dim) / 2;
}

OMEGA_H_INLINE Vector<1> symm2vector(Matrix<1, 1> symm) {
  return vector_1(symm[0][0]);
}

OMEGA_H_INLINE Matrix<1, 1> vector2symm(Vector<1> v) {
  return matrix_1x1(v[0]);
}

OMEGA_H_INLINE Vector<3> symm2vector(Matrix<2, 2> symm) {
  Vector<3> v;
  v[0] = symm[0][0];
  v[1] = symm[1][1];
  v[2] = symm[1][0];
  return v;
}

OMEGA_H_INLINE Matrix<2, 2> vector2symm(Vector<3> v) {
  Matrix<2, 2> symm;
  symm[0][0] = v[0];
  symm[1][1] = v[1];
  symm[1][0] = v[2];
  symm[0][1] = symm[1][0];
  return symm;
}

OMEGA_H_INLINE Vector<6> symm2vector(Matrix<3, 3> symm) {
  Vector<6> v;
  v[0] = symm[0][0];
  v[1] = symm[1][1];
  v[2] = symm[2][2];
  v[3] = symm[1][0];
  v[4] = symm[2][1];
  v[5] = symm[2][0];
  return v;
}

OMEGA_H_INLINE Matrix<3, 3> vector2symm(Vector<6> v) {
  Matrix<3, 3> symm;
  symm[0][0] = v[0];
  symm[1][1] = v[1];
  symm[2][2] = v[2];
  symm[1][0] = v[3];
  symm[2][1] = v[4];
  symm[2][0] = v[5];
  symm[0][1] = symm[1][0];
  symm[1][2] = symm[2][1];
  symm[0][2] = symm[2][0];
  return symm;
}

OMEGA_H_INLINE constexpr Int matrix_ncomps(Int dim) { return dim * dim; }

template <Int dim>
OMEGA_H_INLINE Vector<matrix_ncomps(dim)> matrix2vector(Matrix<dim, dim> m) {
  Vector<matrix_ncomps(dim)> v;
  for (Int i = 0; i < dim; ++i) {
    for (Int j = 0; j < dim; ++j) {
      v[i * dim + j] = m[i][j];
    }
  }
  return v;
}

template <Int dim>
OMEGA_H_INLINE Matrix<dim, dim> vector2matrix(Vector<matrix_ncomps(dim)> v) {
  Matrix<dim, dim> m;
  for (Int i = 0; i < dim; ++i) {
    for (Int j = 0; j < dim; ++j) {
      m[i][j] = v[i * dim + j];
    }
  }
  return m;
}

template <Int n>
OMEGA_H_DEVICE void set_symm(Write<Real> const& a, Int i, Matrix<n, n> symm) {
  set_vector(a, i, symm2vector(symm));
}

template <Int n, typename Arr>
OMEGA_H_DEVICE Matrix<n, n> get_symm(Arr const& a, Int i) {
  return vector2symm(get_vector<symm_ncomps(n)>(a, i));
}

template <Int dim>
OMEGA_H_DEVICE void set_matrix(
    Write<Real> const& a, Int i, Matrix<dim, dim> m) {
  set_vector(a, i, matrix2vector(m));
}

template <Int dim>
OMEGA_H_DEVICE Matrix<dim, dim> get_matrix(Reals const& a, Int i) {
  return vector2symm(get_vector<matrix_ncomps(dim)>(a, i));
}

/* Rodrigues' Rotation Formula */
OMEGA_H_INLINE Matrix<3, 3> rotate(Real angle, Vector<3> axis) {
  return cos(angle) * identity_matrix<3, 3>() + sin(angle) * cross(axis) +
         (1 - cos(angle)) * outer_product(axis, axis);
}

OMEGA_H_INLINE Matrix<2, 2> rotate(Real angle) {
  return matrix_2x2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

OMEGA_H_INLINE Real rotation_angle(Matrix<2, 2> r) { return acos(r[0][0]); }

OMEGA_H_INLINE Real rotation_angle(Matrix<3, 3> r) __attribute__((pure));
OMEGA_H_INLINE Real rotation_angle(Matrix<3, 3> r) {
  return acos((trace(r) - 1.0) / 2.0);
}

OMEGA_H_INLINE Matrix<2, 2> form_ortho_basis(Vector<2> v) {
  Matrix<2, 2> A;
  A[0] = normalize(v);
  A[1] = perp(A[0]);
  return A;
}

OMEGA_H_INLINE Matrix<3, 3> form_ortho_basis(Vector<3> v) {
  Matrix<3, 3> A;
  A[0] = v;
  /* tiny custom code to sort components by absolute value */
  struct {
    Int i;
    Real m;
  } s[3] = {{0, fabs(v[0])}, {1, fabs(v[1])}, {2, fabs(v[2])}};
  if (s[2].m > s[1].m) swap2(s[1], s[2]);
  if (s[1].m > s[0].m) swap2(s[0], s[1]);
  if (s[2].m > s[1].m) swap2(s[1], s[2]);
  /* done, components sorted by increasing magnitude */
  Int lc = s[0].i;
  Int mc = s[1].i;
  Int sc = s[2].i;
  /* use the 2D rotation on the largest components
     (rotate v around the smallest axis) */
  A[1][lc] = -v[mc];
  A[1][mc] = v[lc];
  /* and make the last component zero so that A[0] * A[1] == 0 */
  A[1][sc] = 0;
  A[1] = normalize(A[1]);
  /* now we have 2 orthogonal unit vectors, cross product gives the third */
  A[2] = cross(A[0], A[1]);
  return A;
}

template <Int dim>
Reals repeat_symm(LO n, Matrix<dim, dim> symm);
extern template Reals repeat_symm(LO n, Matrix<3, 3> symm);
extern template Reals repeat_symm(LO n, Matrix<2, 2> symm);

Reals resize_symms(Reals old_symms, Int old_dim, Int new_dim);

}  // end namespace Omega_h

#endif
