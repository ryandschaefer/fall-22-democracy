# Library to read C++
library(Rcpp)

# Import Jane Austen dataset
library(dplyr)
library(janeaustenr)
library(tidytext)
data = austen_books() %>%
  unnest_tokens(word, text) %>%
  count(book, word, sort = TRUE)
head(data)

# Create C++ function with rcpp
setwd("~/Documents/SMU/DigitalHistoryInternship/fall-22-democracy/JSD")
sourceCpp("./JSD.cpp")

# Call log likelihood function
jsd = JSD(data, group = "book", g1 = "Pride & Prejudice", g2 = "Emma")
jsd

# Rcpp.package.skeleton("JSD", cpp_files = c("JSD".cpp), example_code = FALSE)

