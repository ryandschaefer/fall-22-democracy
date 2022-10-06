import pandas as pd

# These will let us use R packages:
from rpy2.robjects.packages import importr
from rpy2.robjects import pandas2ri

# Convert pandas.DataFrames to R dataframes automatically.
pandas2ri.activate()

# Load LogLikelihood package
LogLikelihood = importr("LogLikelihood")

# Import dataset
data = pd.read_csv("austen.csv")
print(data.head())

# Run log likelihood
ll = LogLikelihood.log_likelihood(data, group = "book")
print(ll.head())