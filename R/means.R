#' @export
means_narm <- function(chunks) {
  .Call(`dice_means_narm`, chunks)
}

#' @export
means_narm_parallel <- function(chunks) {
  .Call(`dice_means_narm_parallel`, chunks)
}

#' @export
means_narm_chop <- function(x, rows) {
  .Call(`dice_means_narm_chop`, x, rows)
}
