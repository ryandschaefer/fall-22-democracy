# Download dataset
require(devtools)
install_github("stephbuon/hansardr")
library(hansardr)
data("hansard_1850")
data("speaker_metadata_1850")

# Merge datasets
library(tidyverse)
h1850 = hansard_1850 %>%
  inner_join(speaker_metadata_1850)
head(h1850)

# Format data
library(tidytext)
data = h1850 %>%
  unnest_tokens(word, text) %>%
  count(speaker, word, sort = TRUE)
head(data)

# Install log likelihood
setwd("./LogLikelihood")
install.packages("LogLikelihood_1.0.tar.gz", type = "source", repos = NULL)
library(LogLikelihood)

# Run log likelihood
ll = data %>%
  log_likelihood(group = "decade", group_list = c("1870", "1880"))
ll

# Create visualization
library(ggplot2)
library(scales)
ll %>%
  ggplot(aes(x = X1870, y = X1880, label = word)) +
    geom_point() +
    geom_text(size = 3, position = position_jitter()) + 
    labs(
      x = "1870",
      y = "1880",
      title = "Log Likelihood in Parliament in 1870 and 1880"
    ) + 
    scale_x_log10(breaks = trans_breaks("log10", function(x) 10^x),
                  labels = trans_format("log10", math_format(10^.x))) +
    scale_y_log10(breaks = trans_breaks("log10", function(x) 10^x),
                  labels = trans_format("log10", math_format(10^.x)))

