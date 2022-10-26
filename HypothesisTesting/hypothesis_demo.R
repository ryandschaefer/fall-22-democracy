library(LogLikelihood)

# Function to conduct a hypothesis test
twoSampleTest = function(df, group1 = NULL, group2 = NULL, word_list = c(), alternative = c("two.sided", "less", "greater")) {
  # If a word list was provided, create subset of df with words list
  if (length(word_list) > 0) {
    df = df %>% filter(word %in% word_list)
  }
  
  # If group1 is null, choose group
  if (is.null(group1)) {
    if (!is.null(group2) && names(df)[2] == group2) {
      # If first column name is group2, make group1 second column name
      group1 = names(df[3])
    } else {
      # Else, make group1 first column name
      group1 = names(df[2])
    }
  }
  
  # If group2 is null, choose group
  if (is.null(group2)) {
    if (names(df)[2] == group1) {
      # If first column name is group1, make group2 second column name
      group2 = names(df[3])
    } else {
      # Else, make group2 first column name
      group2 = names(df[2])
    }
  }
  
  # Conduct t-test
  testResult = t.test(df[[group1]], df[[group2]], alternative = alternative)
  
  # Print output
  if (testResult$alternative == "two.sided") {
    # If two sided test, set alternative to "different"
    alternative = "different"
  } else {
    # Else, set alternative to test alternative
    alternative = testResult$alternative
  }
  cat (
    "The probability of getting a test statistic as or more extreme than",
    testResult$statistic,
    "is",
    testResult$p.value,
    "if the mean log likelihood score of",
    group1,
    "is the same as the log likelihood score of",
    group2,
    "\n\n"
  )
  if (testResult$p.value <= 0.05) {
    # Reject null
    cat (
      "Thus, there is sufficient evidence to conclude that the mean log likelihood score of",
      group1,
      "is",
      alternative,
      "than the score of",
      group2,
      "\n"
    )
  } else {
    # Fail to reject null
    # Reject null
    cat (
      "Thus, there is not sufficient evidence to conclude that the mean log likelihood score of",
      group1,
      "is",
      alternative,
      "than the score of",
      group2,
      "\n"
    )
  }
  
  # Plot distribution
  curve(
    dt(x, df=testResult$parameter), 
    from=-4, to=4, 
    main = 'Null Distribution and Test Statistic', 
    ylab = 'Probability Density'
  ) 
  abline(v = testResult$statistic, col = "red")
  
  # Return test results
  return(testResult)
}

# Import dataset
setwd("../")
austen = read.csv("austen.csv", header = TRUE)
head(austen)

# Compute log likelihood
ll = log_likelihood(austen, group = "book")
head(ll)

twoSampleTest(ll)
