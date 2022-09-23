\name{LogLikelihood-package}
\alias{LogLikelihood-package}
\alias{LogLikelihood}
\docType{package}
\title{
  Log Likelihood
}
\description{
  This packages contains a function called log_likelihood that will calculate word distinctiveness using the log likelihood algorithm.
}
\details{
  The function input is a data frame with columns for the word, the text group, and the number of times that word appears in that group. The column names are set to "word", "group", and "n" by default but they can be changed using the parameters word, group, and n. If any of these columns are not found, the function will not work. The output will be a new data frame with a column called "word" containing all unique words and subsequent columns for all unique groups with the name of that group. The data frame will contain the log likelihood scores for each word in each group. The larger a log likelihood score is, the more distinctive that word is to that group.
}
\author{
  Ryan Schaefer
}
\keyword{ package }
\examples{
# Load example Jane Austen dataset
require(dplyr)
require(janeaustenr)
require(tidytext)
data = austen_books() \%>\%
    unnest_tokens(word, text) \%>\%
    count(book, word, sort = TRUE)
# View example dataset
head(data)

# Compute log likelihood
output = log_likelihood(data, group = "book")
head(output)
}
