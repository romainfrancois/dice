#' Dice
#'
#' @param data A tibble
#' @param x A column of `data`
#'
#' @export
dice <- function(data, x) {
  UseMethod("dice")
}

#' @export
dice.data.frame <- function(data, x) {
  list(x)
}

#' @importFrom dplyr group_rows
#' @importFrom purrr map
#' @importFrom vctrs vec_slice
#' @export
dice.grouped_df <- function(data, x) {
  rows <- group_rows(data)
  map(rows, vec_slice, x = x)
}

#' @rdname dice
#' @export dice.rowwise_df
#' @method dice rowwise_df
dice.rowwise_df <- function(data, x) {
  UseMethod("dice.rowwise_df", x)
}

#' @method dice.rowwise_df default
#' @export
dice.rowwise_df.default <- function(data, x) {
  map(seq_along(nrow(data)), vec_slice, x = x)
}

#' @method dice.rowwise_df list
#' @export
dice.rowwise_df.list <- function(data, x) {
  map(seq_along(nrow(data)), function(i) vec_slice(x, i)[[1L]])
}

#' @method dice.rowwise_df data.frame
#' @export
dice.rowwise_df.data.frame <- function(data, x) {
  map(seq_along(nrow(data)), vec_slice, x = x)
}
