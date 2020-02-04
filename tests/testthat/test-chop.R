test_that("chop() works for ungrouped data frames", {
  df <- tibble(x = 1:3, y = list(1, 2, 3), z = data.frame(1:3))
  expect_equal(chop(df, df$x), list(df$x))
  expect_equal(chop(df, df$y), list(df$y))
  expect_equal(chop(df, df$z), list(df$z))
})

test_that("chop() works for grouped data frames", {
  df <- tibble(x = 1:4, y = list(1, 2, 3, 4), z = data.frame(a = 1:4), g = c(1,1,2,2)) %>% group_by(g)
  expect_equal(chop(df, df$x), list(1:2, 3:4))
  expect_equal(chop(df, df$y), list(list(1, 2), list(3, 4)))
  expect_equal(chop(df, df$z), list(data.frame(a = 1:2), data.frame(a = 3:4)))
})

test_that("chop() works for rowwise data frames", {
  df <- tibble(x = 1:2, y = list(1, 2), z = data.frame(a = 1:2)) %>% rowwise()
  expect_equal(chop(df, df$x), list(1L, 2L))
  expect_equal(chop(df, df$y), list(1, 2))
  expect_equal(chop(df, df$z), list(data.frame(a = 1L), data.frame(a = 2L)))
})
