
#' Lazily chop columns of a data frame
#'
#' @param data A data frame
#' @param callback Callback function
#'
#' @examples
#' e <- iris %>%
#'   dplyr::group_by(Species) %>%
#'   dice()
#'
#' e$Sepal.Length
#' e$Sepal.Width
#'
#' # Using callback ----------------------------------------------------------
#'
#' e <- iris %>%
#'   dplyr::group_by(Species) %>%
#'   dice(callback = print)
#'
#' chunks <- e$Sepal.Width
#'
#' @importFrom rlang env chr_unserialise_unicode env_bind_lazy set_names as_function expr
#' @importFrom purrr pmap
#' @export
dice <- function(data, callback = NULL) {
  e <- env()
  names <- chr_unserialise_unicode(names(data))

  promises <- if (is.null(callback)) {
    map(data, ~expr({
      chop(!!data, !!.x)
    }))
  } else {
    callback <- as_function(callback)
    pmap(list(data, names, seq_along(data)), ~expr({
      res <- chop(!!data, !!..1)
      callback(list(index = !!..3, name = !!..2, chunks = res))
      res
    }))
  }

  env_bind_lazy(e, !!!set_names(promises, names))
  e
}
