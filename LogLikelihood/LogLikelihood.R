# Library to read C++
library(Rcpp)

# Import Jane Austen dataset
library(dplyr)
library(janeaustenr)
library(tidytext)
data = austen_books() %>%
  unnest_tokens(word, text) %>%
  count(book, word, sort = TRUE)

# Create C++ function with rcpp
sourceCpp("./LogLikelihood.cpp")

# Call log likelihood function
ll_scores = log_likelihood(data, group = "book")
ll_scores

# Set column names to default
colnames(data) = c("group", "word", "n")
# Call log likelihood function
ll_scores = log_likelihood(data)
ll_scores

Rcpp.package.skeleton("LogLikelihood", cpp_files = c("LogLikelihood.cpp"), example_code = FALSE)

