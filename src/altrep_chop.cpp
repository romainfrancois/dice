#include "dice.h"
#include <R_ext/Altrep.h>
#include <tbb/tbb.h>

struct dice_chop_dbl {

  static R_altrep_class_t class_t;

  static SEXP Make(SEXP host, int start, int n){
    SEXP bounds = PROTECT(Rf_allocVector(INTSXP, 2));
    INTEGER0(bounds)[0] = start;
    INTEGER0(bounds)[1] = start + n;
    SEXP res = R_new_altrep(class_t, host, bounds);
    UNPROTECT(1);
    return res;
  }

  // ALTREP methods -------------------

  // The length of the object
  static R_xlen_t Length(SEXP vec){
    int* p_bounds = INTEGER(R_altrep_data2(vec));
    return p_bounds[1] - p_bounds[0];
  }

  // What gets printed when .Internal(inspect()) is used
  static Rboolean Inspect(SEXP x, int pre, int deep, int pvec, void (*inspect_subtree)(SEXP, int, int, int)){
    Rprintf("REALSXP dice_chop_dbl (len=%d)\n", Length(x));
    return (Rboolean)TRUE;
  }

  // ALTVEC methods ------------------

  // The start of the data, i.e. the underlying double* array from the std::vector<double>
  //
  // This is guaranteed to never allocate (in the R sense)
  static const void* Dataptr_or_null(SEXP vec){
    return static_cast<const void*>(REAL(R_altrep_data1(vec)) + INTEGER_ELT(R_altrep_data2(vec), 0));
  }

  // same in this case, writeable is ignored
  static void* Dataptr(SEXP vec, Rboolean writeable){
    return static_cast<void*>(REAL(R_altrep_data1(vec)) + INTEGER_ELT(R_altrep_data2(vec), 0));
  }


  // ALTREAL methods -----------------

  static double real_Elt(SEXP vec, R_xlen_t i){
    return REAL_ELT(R_altrep_data1(vec), INTEGER_ELT(R_altrep_data2(vec), 0) + i);
  }


  // -------- initialize the altrep class with the methods above

  static void Init(DllInfo* dll){
    class_t = R_make_altreal_class("dice_chop_dbl", "dice", dll);

    // altrep
    R_set_altrep_Length_method(class_t, Length);
    R_set_altrep_Inspect_method(class_t, Inspect);

    // altvec
    R_set_altvec_Dataptr_method(class_t, Dataptr);
    R_set_altvec_Dataptr_or_null_method(class_t, Dataptr_or_null);

    // altreal
    R_set_altreal_Elt_method(class_t, real_Elt);
  }

};

R_altrep_class_t dice_chop_dbl::class_t;

void init_dice_vec_parallel_chop_altrep(DllInfo* dll){
  dice_chop_dbl::Init(dll);
}

SEXP dice_vec_parallel_chop_altrep(SEXP x, SEXP indices) {
  R_xlen_t n = XLENGTH(indices);
  SEXP res = PROTECT(Rf_allocVector(VECSXP, n));

  // allocate a vector as big as x
  SEXP host = PROTECT(Rf_allocVector(REALSXP, XLENGTH(x)));

  // where each group starts in the host
  SEXP starts = PROTECT(Rf_allocVector(INTSXP, n));

  // Make the n alrep objects
  auto make_alrep_chops = [=]() {
    R_xlen_t k = 0;
    for (R_xlen_t i=0; i<n; i++) {
      R_xlen_t n = XLENGTH(VECTOR_ELT(indices, i));
      SET_VECTOR_ELT(res, i,
        dice_chop_dbl::Make(host, k, n)
      );
      k += n;
    }
  };

  // copy data from x to the right slice in the host vector
  auto sprinkle_data = [=](){
    // track where each group starts
    int* p_starts = INTEGER(starts);
    int k = 0;
    for (R_xlen_t i=0; i<n; i++) {
      p_starts[i] = k;
      k += XLENGTH(VECTOR_ELT(indices, i));
    }

    // then fill in parallel
    tbb::parallel_for(R_xlen_t(0), n, [=](R_xlen_t i) {
      SEXP indices_i = VECTOR_ELT(indices, i);
      R_xlen_t n_i = XLENGTH(indices_i);
      int* p_indices = INTEGER(indices_i);
      double* p_x = REAL(x);
      double* p_res = REAL(host) + INTEGER_ELT(starts, i);

      for (R_xlen_t j=0; j<n_i; j++, ++p_res, ++p_indices) {
        *p_res = p_x[*p_indices - 1];
      }
    });

  };

  tbb::parallel_invoke(make_alrep_chops, sprinkle_data);

  UNPROTECT(3);
  return res;
}
