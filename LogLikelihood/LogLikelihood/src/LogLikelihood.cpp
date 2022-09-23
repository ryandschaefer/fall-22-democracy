// C++ dependencies
#include <map>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

// Rcpp
#include <Rcpp.h>
using namespace Rcpp;

// Private variables
DataFrame text;
// Map to hold final results
DataFrame results;
// Vector of all groups
CharacterVector groups;
// Vector of all words
CharacterVector words;
// Map to hold word and group count
map<String, map<String, int>> counts;
// Map to count the number of words in each group
map<String, int> groupCount;
// Map to hold the total number of times each word appears
map<String, int> wordCount;
// Total word count
int totalCount;
// String variables with input column names
String Group;
String Word;
String Count;

// Calculate the log likelihood score for one word in one group
double calcLL(const String& g, const String& w) {
    // Variable for word count in group
    int a;
    // Check if group and word combo doesn't exist in counts map
    if (counts[g].find(w) == counts[g].end()) {
        // If not found, set a = 0
        a = 0;
    } else {
        // If found, set a = value in map
        a = counts[g][w];
    }
    // Word count in remaining groups
    int b = wordCount[w] - a;
    // Remaining word count in group
    int c = groupCount[g] - a;
    // Remaining word count in remaining groups
    int d = totalCount - a - b - c;

    // Calculate E1
    double E1 = (double)((a + c) * (a + b)) / totalCount;
    // Calculate E2
    double E2 = (double)((b + d) * (a + b)) / totalCount;

    // Initialize log likelihood to 0
    double LL = 0.0;
    // If word count in group is greater than 0, calculate first part of log likelihood
    if (a > 0) {
        LL = 2 * a * log(a / E1);
    }

    // If word count in remaining groups is greater than 0, calculate second part of log likelihood
    if (b > 0) {
        LL += 2 * b * log(b / E2);
    }

    // Return result
    return LL;
}

// Calculate the log likelihood score for each word in each group
void calcLL() {
    // Create first column of result for word
    results = DataFrame::create(Named("word") = words);
    // Loop through groups
    for (int i = 0; i < groups.size(); i++) {
        // Numeric vector to hold LL scores for group
        NumericVector scores;
        // Loop through words
        for (int j = 0; j < words.size(); j++) {
            // Call calcLL for group and word combo
            double ll = calcLL(groups[i], words[j]);
            scores.push_back(ll);
        }
        String g = groups[i];
        results.push_back(scores, g);
    }
}

// Calculate words and groups
void getCounts() {
    // Get vectors for group, word, and count
    CharacterVector allGroups = text[Group];
    CharacterVector allWords = text[Word];
    NumericVector allCounts = text[Count];
    // Set total count to 0
    totalCount = 0;
    // Loop through text
    for (int i = 0; i < text.nrows(); i++) {
        // Get group of current row
        String g = allGroups[i];
        // Get word of current row
        String w = allWords[i];
        // Get count of current row
        int c = allCounts[i];
        // Add count to current group's count
        groupCount[g] += c;
        // Add count to current word's count
        wordCount[w] += c;
        // Add count to totalCount
        totalCount += c;
        // Add count of group and word combo to counts
        counts[g][w] = c;
    }
}

// Get all unique groups and words
void getGroupsWords() {
    // Get all unique words
    words = text[Word];
    words = unique(words);
    // Loop through words
    for (int i = 0; i < words.size(); i++) {
        // Add all words to map with count of 0
        wordCount[words[i]] = 0;
    }
    // Get all unique groups
    groups = text[Group];
    groups = unique(groups);
    // Loop through groups
    for (int i = 0; i < groups.size(); i++) {
        // Add all groups to map with count of 0
        groupCount[groups[i]] = 0;
    }
}

// Function to compute log likelihood
// Input a data frame with columns for word, group, and n (word count in group)
// Outputs a data frame with a column for all unique words
// Subsequent columns contain LL scores for a given word for all unique groups
//' Log Likelihood
//' @description Calculates word distinctiveness using the log likelihood algorithm. You input a data frame with columns for the word, the text group, and the number of times that word appears in that group. The column names are set to "word", "group", and "n" by default but they can be changed using the parameters word, group, and n. If any of these columns are not found, the function will not work. The output will be a new data frame with a column called "word" containing all unique words and subsequent columns for all unique groups with the name of that group. The data frame will contain the log likelihood scores for each word in each group. The larger a log likelihood score is, the more distinctive that word is to that group.
//' @param t Data frame containing data
//' @param group Name of data frame column containing text group
//' @param word Name of data frame column containing words
//' @param n Name of data frame column containing word count in text group
//' @return Data frame containing a column containing unique words and columns for log likelihood scores for each group
//' @examples
//' # Load example Jane Austen dataset
//' require(dplyr)
//' require(janeaustenr)
//' require(tidytext)
//' data = austen_books() %>%
//'     unnest_tokens(word, text) %>%
//'     count(book, word, sort = TRUE)
//' # View example dataset
//' head(data)
//' 
//' # Compute log likelihood
//' output = log_likelihood(data, group = "book")
//' head(output)
//' @useDynLib LogLikelihood
//' @importFrom Rcpp evalCpp
//' @exportPattern ^[[:alpha:]]+
// [[Rcpp::export]]
DataFrame log_likelihood(DataFrame t, String group = "group", String word = "word", String n = "n") {
    // Assign value to text private variable
    text = t;
    // Assign column names
    Group = group;
    Word = word;
    Count = n;

    // Get unique groups and words
    getGroupsWords();

    // Get group and word counts
    getCounts();
    // Calculate log likelihood scores
    calcLL();

    // Clear maps
    groupCount.clear();
    wordCount.clear();
    counts.clear();

    // Return result
    return results;
}