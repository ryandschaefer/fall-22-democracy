# Library to read C++
library(Rcpp)

# Import Jane Austen dataset
library(dplyr)
library(janeaustenr)
library(tidytext)
data = austen_books() %>%
  unnest_tokens(word, text) %>%
  count(book, word, sort = TRUE)
colnames(data) = c("group", "word", "n")

# Create C++ function with rcpp
sourceCpp("./LogLikelihood.cpp")

# Call log likelihood function
ll_scores = log_likelihood(data)
ll_scores