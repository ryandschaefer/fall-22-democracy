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

# Run log likelihood
setwd("~/Documents/SMU/DigitalHistoryInternship/fall-22-democracy/LogLikelihood")
install.packages("LogLikelihood_1.0.tar.gz", type = "source", repos = NULL)
library(LogLikelihood)
ll = log_likelihood(data, group = "book")
ll
library(ggplot2)
ggplot(ll, aes(x = Emma, y = Persuasion, label = word)) +
  geom_point() +
  geom_text(hjust=0, vjust=0)

# Run JSD
setwd("../JSD")
sourceCpp("./JSD.cpp")
jsd = JSD(data, group = "book", g1 = "Pride & Prejudice", g2 = "Emma")
jsd

