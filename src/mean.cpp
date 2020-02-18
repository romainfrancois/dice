#include "dice.h"
#include <tbb/tbb.h>

SEXP means_narm(SEXP chunks) {
  R_xlen_t n = XLENGTH(chunks);

  SEXP result = PROTECT(Rf_allocVector(REALSXP, n));
  double* p_res = REAL(result);

  for (R_xlen_t i=0; i<n; i++, ++p_res) {
    SEXP x = VECTOR_ELT(chunks, i);
    double* p_x = REAL(x);
    int n_x = XLENGTH(x), m_x = n_x;

    long double res = 0.0;
    for (R_xlen_t j = 0; j < n_x; j++, ++p_x) {
      if (R_IsNA(*p_x)) {
        m_x--;
      } else {
        res += *p_x;
      }
    }
    if (m_x == 0) {
      res = R_NaN;
    } else {
      long double t = 0.0;
      p_x = REAL(x);
      for (R_xlen_t j = 0; j < n_x; j++, ++p_x) {
        t += R_IsNA(*p_x) ? 0.0 : (*p_x - res);
      }
      res += (t / m_x);
    }

    *p_res = (double)res;
  }

  UNPROTECT(1);
  return result;
}

SEXP means_narm_parallel(SEXP chunks) {
  R_xlen_t n = XLENGTH(chunks);

  SEXP result = PROTECT(Rf_allocVector(REALSXP, n));
  double* p_res = REAL(result);

  tbb::parallel_for(R_xlen_t(0), n, [=](R_xlen_t i) {
    // for (R_xlen_t i=0; i<n; i++, ++p_res) {
    SEXP x = VECTOR_ELT(chunks, i);
    double* p_x = REAL(x);
    int n_x = XLENGTH(x), m_x = n_x;

    long double res = 0.0;
    for (R_xlen_t j = 0; j < n_x; j++, ++p_x) {
      if (R_IsNA(*p_x)) {
        m_x--;
      } else {
        res += *p_x;
      }
    }
    if (m_x == 0) {
      res = R_NaN;
    } else {
      long double t = 0.0;
      p_x = REAL(x);
      for (R_xlen_t j = 0; j < n_x; j++, ++p_x) {
        t += R_IsNA(*p_x) ? 0.0 : (*p_x - res);
      }
      res += (t / m_x);
    }

    p_res[i] = (double)res;
  });

  UNPROTECT(1);
  return result;
}

SEXP means_narm_chop(SEXP vec, SEXP rows) {
  R_xlen_t n = XLENGTH(rows);

  SEXP result = PROTECT(Rf_allocVector(REALSXP, n));
  double* p_res = REAL(result);
  double* p_vec = REAL(vec);
  for (R_xlen_t i=0; i<n; i++, ++p_res) {
    SEXP rows_i = VECTOR_ELT(rows, i);
    int* p_rows_i = INTEGER(rows_i);
    int n_x = XLENGTH(rows_i), m_x = n_x;

    long double res = 0.0;
    for (R_xlen_t j = 0; j < n_x; j++, ++p_rows_i) {
      double value = p_vec[*p_rows_i - 1];
      if (R_IsNA(value)) {
        m_x--;
      } else {
        res += value;
      }
    }

    if (m_x == 0) {
      res = R_NaN;
    } else {
      long double t = 0.0;
      p_rows_i = INTEGER(rows_i);
      for (R_xlen_t j = 0; j < n_x; j++, ++p_rows_i) {
        double value = p_vec[*p_rows_i - 1];
        t += R_IsNA(value) ? 0.0 : (value - res);
      }
      res += t / m_x;
    }

    *p_res = (double)res;
  }

  UNPROTECT(1);
  return result;
}


