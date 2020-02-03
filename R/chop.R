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

#' @importFrom dplyr group_rows
#' @importFrom purrr map
#' @importFrom vctrs vec_slice
#' @export
chop.grouped_df <- function(data, x) {
  rows <- group_rows(data)
  map(rows, vec_slice, x = x)
}

#' @rdname chop
#' @export chop.rowwise_df
#' @method chop rowwise_df
chop.rowwise_df <- function(data, x) {
  UseMethod("chop.rowwise_df", x)
}

#' @method chop.rowwise_df default
#' @export
chop.rowwise_df.default <- function(data, x) {
  map(seq_along(nrow(data)), vec_slice, x = x)
}

#' @method chop.rowwise_df list
#' @export
chop.rowwise_df.list <- function(data, x) {
  map(seq_along(nrow(data)), function(i) vec_slice(x, i)[[1L]])
}

#' @method chop.rowwise_df data.frame
#' @export
chop.rowwise_df.data.frame <- function(data, x) {
  map(seq_along(nrow(data)), vec_slice, x = x)
}
