#' @export
vec_parallel_chop_dbl <- function(x, indices = NULL) {
  .Call(`dice_vec_parallel_chop`, x, indices)
}

#' @export
vec_parallel_chop_dbl_allocate <- function(x, indices = NULL) {
  .Call(`dice_vec_parallel_chop_allocate`, x, indices)
}

#' @export
vec_parallel_chop_altrep1 <- function(x, indices = NULL) {
  .Call(`dice_vec_parallel_chop_altrep1`, x, indices)
}

#' @export
vec_parallel_chop_altrep_mean <- function(x, indices = NULL) {
  .Call(`dice_vec_parallel_chop_altrep_mean`, x, indices)
}

#' @importFrom rlang is_double
#' @export
vec_parallel_chop <- function(x, indices = NULL) {
  if (is_double(x)) {
    vec_parallel_chop_altrep1(x, indices)
  } else {
    vec_chop(x, indices)
  }
}

#' Chop a vector along the groups of a data frame
#'
#' @param data A tibble
#' @param x A vector, most often a column of `data`
#'
#' @return a list of slices of `x` with one element per group of `data`
#'
#' @examples
#' gdf <- iris %>%
#'   dplyr::group_by(Species)
#'
#' chop(gdf, iris$Sepal.Length)
#' chop(gdf, 1:150)
#'
#' @export
chop <- function(data, x) {
  UseMethod("chop")
}

#' @export
chop.data.frame <- function(data, x) {
  list(x)
}

#' @importFrom purrr map
#' @importFrom vctrs vec_slice vec_chop
#' @export
chop.grouped_df <- function(data, x) {
  vec_parallel_chop(x, dplyr::group_rows(data))
}

#' @rdname chop
#' @export chop.rowwise_df
#' @method chop rowwise_df
#' @export
chop.rowwise_df <- function(data, x) {
  UseMethod("chop.rowwise_df", x)
}

#' @method chop.rowwise_df default
#' @export
chop.rowwise_df.default <- function(data, x) {
  vec_parallel_chop(x)
}

#' @method chop.rowwise_df list
#' @export
chop.rowwise_df.list <- function(data, x) {
  map(vec_parallel_chop(x), `[[`, 1L)
}

#' @method chop.rowwise_df data.frame
#' @export
chop.rowwise_df.data.frame <- function(data, x) {
  vec_parallel_chop(x)
}
