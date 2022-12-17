# Download dataset
require(devtools)
install_github("stephbuon/hansardr")
library(hansardr)

# Merge datasets
library(tidyverse)
hansard = hansard_join(files = c("hansard", "debate_metadata"))
hansard$decade = paste(substr(hansard$speechdate, 1, 3), "0", sep = "")
head(hansard)

# Format data
library(tidytext)
data = hansard %>%
  unnest_tokens(word, text) %>%
  count(decade, word, sort = TRUE)
# Remove stopwords
library(tm)
stopWords = stopwords("en")
data = data %>% filter(!(word %in% stopWords))
head(data)

# Import dhmeasures
setwd("../")
install.packages("dhmeasures_1.0.tar.gz", type = "source", repos = NULL)
library(dhmeasures)

# Run log likelihood
ll = data %>%
  log_likelihood(group = "decade", group_list = c("1870", "1880"))
head(ll)

# Create visualization
library(ggplot2)
library(scales)
ll %>%
  ggplot(aes(x = X1870, y = X1880, label = word)) +
    geom_point() +
    geom_text(hjust=0, vjust=0) + 
    labs(
      x = "1870",
      y = "1880",
      title = "Log Likelihood in Parliament in 1870 and 1880"
    ) + 
    scale_x_log10(breaks = trans_breaks("log10", function(x) 10^x),
                  labels = trans_format("log10", math_format(10^.x))) +
    scale_y_log10(breaks = trans_breaks("log10", function(x) 10^x),
                  labels = trans_format("log10", math_format(10^.x)))

