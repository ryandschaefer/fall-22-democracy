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
ll = log_likelihood(data, group = "speaker")
ll = ll %>%
  filter(!is.nan(MR.GLADSTONE) & !is.nan(MR.DISRAELI))
ll

# Create visualization
library(ggplot2)
ggplot(ll, aes(x = log(MR.GLADSTONE), y = log(MR.DISRAELI), label = word)) +
  geom_point() +
  geom_text(size = 3, nudge_y = 0.5)

temp = data %>%
  filter(speaker == "MR. GLADSTONE" | speaker == "MR. DISRAELI") %>%
  pivot_wider(names_from = "speaker", values_from = "n") %>%
  rename(gladstone = `MR. GLADSTONE`, disraeli = `MR. DISRAELI`) %>%
  filter(!is.na(gladstone) & !is.na(disraeli)) %>%
  mutate(prop_g = gladstone / (gladstone + disraeli), prop_d = disraeli / (gladstone + disraeli), diff = abs(prop_g - prop_d)) %>%
  arrange(desc(diff))
temp

ll %>%
  select(word, MR.GLADSTONE, MR.DISRAELI) %>%
  mutate(log_g = log(MR.GLADSTONE), log_d = log(MR.DISRAELI)) %>%
  filter(word == "divorce")



