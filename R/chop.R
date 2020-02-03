#' chop
#'
#' @param data A tibble
#' @param x A column of `data`
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

#' @method dice.rowwise_df default
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
