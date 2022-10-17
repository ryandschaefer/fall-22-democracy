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
# Export demo dataset
setwd("~/Documents/SMU/DigitalHistoryInternship/fall-22-democracy")
write.csv(data, "./austen.csv", row.names = FALSE)

# Run log likelihood
setwd("./LogLikelihood")
install.packages("LogLikelihood_1.0.tar.gz", type = "source", repos = NULL)
library(LogLikelihood)
ll = log_likelihood(data, group = "book")
ll
library(ggplot2)
ggplot(ll, aes(x = log(Emma), y = log(Persuasion), label = word)) +
  geom_point() +
  geom_text(hjust=0, vjust=0)
setwd("../")

# Run JSD
setwd("./JSD")
sourceCpp("./JSD.cpp")
# Default
jsd_out = jsd(data, group = "book")
jsd_out
# Provide word list
jsd_out = jsd(data, group = "book", word_list = c("person", "age"))
jsd_out
# Provide group list
jsd_out = jsd(data, group = "book", group_list = c("Mansfield Park", "Emma", "Pride & Prejudice"))
jsd_out
# Provide word and group lists
jsd_out = jsd(data, group = "book", group_list = c("Mansfield Park", "Emma", "Pride & Prejudice"), word_list = c("person", "age"))
jsd_out
# Cumulative
jsd_out = jsd(data, group = "book", cumulative = TRUE)
jsd_out
