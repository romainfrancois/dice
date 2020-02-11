#include "dice.h"
#include <tbb/tbb.h>

SEXP dice_vec_parallel_chop(SEXP x, SEXP indices) {
  R_xlen_t n = XLENGTH(indices);
  SEXP res = PROTECT(Rf_allocVector(VECSXP, n));

  // allocate all the chunks serially
  for (R_xlen_t i = 0; i < n; i++) {
    SET_VECTOR_ELT(res, i, Rf_allocVector(REALSXP, XLENGTH(VECTOR_ELT(indices, i))));
  }

  // fill in parallel
  tbb::parallel_for(R_xlen_t(0), n, [=](R_xlen_t i) {
    SEXP indices_i = VECTOR_ELT(indices, i);
    R_xlen_t n_i = XLENGTH(indices_i);
    double* p_res_i = REAL(VECTOR_ELT(res, i));
    int* p_indices_i = INTEGER(indices_i);

    double* p_x = REAL(x);

    for (R_xlen_t j=0; j<n_i; j++, ++p_res_i, ++p_indices_i) {
      *p_res_i = p_x[*p_indices_i - 1];
    }

  });

  UNPROTECT(1);
  return res;
}

SEXP dice_vec_parallel_chop_allocate(SEXP x, SEXP indices) {
  R_xlen_t n = XLENGTH(indices);
  SEXP res = PROTECT(Rf_allocVector(VECSXP, n));

  // allocate all the chunks serially
  for (R_xlen_t i = 0; i < n; i++) {
    SET_VECTOR_ELT(res, i, Rf_allocVector(REALSXP, XLENGTH(VECTOR_ELT(indices, i))));
  }

  UNPROTECT(1);
  return res;
}
