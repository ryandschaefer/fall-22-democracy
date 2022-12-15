# Download dataset
require(devtools)
install_github("stephbuon/hansardr")
library(hansardr)

# Create 1870 and 1880 dataset
rawdata = hansard_join(files = c("hansard", "debate_metadata"))
rawdata = rawdata %>% mutate(decade = paste(substr(speechdate, 1, 3), "0", sep = ""))
rawdata

# Format data
library(tidytext)
data = rawdata %>%
  unnest_tokens(word, text) %>%
  count(decade, word, sort = TRUE)
head(data)

# Stopword text
library(tm)
stopWords = stopwords("en")
data = data %>%
  filter(!(word %in% stopWords))
head(data)

# Install log likelihood
setwd("./LogLikelihood")
install.packages("LogLikelihood_1.0.tar.gz", type = "source", repos = NULL)
library(LogLikelihood)

# Run log likelihood
ll = log_likelihood(data, group = "decade", group_list = c("1870", "1880"))
ll

# Create visualization
library(ggplot2)
ggplot(ll, aes(x = X1870, y = X1880, label = word)) +
  geom_point() +
  geom_text(size = 3, nudge_y = 0.5) + 
  labs(
    x = "1870",
    y = "1880",
    title = "Log Likelihood in Parliament in 1870 and 1880"
  )




