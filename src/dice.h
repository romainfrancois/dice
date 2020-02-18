#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

void init_dice_vec_parallel_chop_altrep(DllInfo*);

SEXP dice_vec_parallel_chop(SEXP x, SEXP indices);
SEXP dice_vec_parallel_chop_allocate(SEXP x, SEXP indices);
SEXP dice_vec_parallel_chop_altrep(SEXP x, SEXP indices);

SEXP means_narm(SEXP);
SEXP means_narm_chop(SEXP, SEXP);
