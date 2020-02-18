#include "dice.h"

static const R_CallMethodDef CallEntries[] = {
  {"dice_vec_parallel_chop", (DL_FUNC)& dice_vec_parallel_chop, 2},
  {"dice_vec_parallel_chop_allocate", (DL_FUNC)& dice_vec_parallel_chop_allocate, 2},
  {"dice_vec_parallel_chop_altrep1", (DL_FUNC)& dice_vec_parallel_chop_altrep, 2},

  {"dice_means_narm", (DL_FUNC)& means_narm, 1},
  {"dice_means_narm_chop", (DL_FUNC)& means_narm_chop, 2},

  {NULL, NULL, 0}
};

extern "C" void R_init_dice(DllInfo* dll) {
  init_dice_vec_parallel_chop_altrep(dll);

  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
