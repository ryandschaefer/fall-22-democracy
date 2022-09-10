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

// Calculate the log likelihood score for one word in one group
double calcLL(const String& group, const String& word) {
    // Variable for word count in group
    int a;
    // Check if group and word combo doesn't exist in counts map
    if (counts[group].find(word) == counts[group].end()) {
        // If not found, set a = 0
        a = 0;
    } else {
        // If found, set a = value in map
        a = counts[group][word];
    }
    // Word count in remaining groups
    int b = wordCount[word] - a;
    // Remaining word count in group
    int c = groupCount[group] - a;
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
        String group = groups[i];
        results.push_back(scores, group);
    }
}

// Calculate words and groups
void getCounts() {
    // Get vectors for group, word, and count
    CharacterVector allGroups = text["group"];
    CharacterVector allWords = text["word"];
    NumericVector allCounts = text["n"];
    // Set total count to 0
    totalCount = 0;
    // Loop through text
    for (int i = 0; i < text.nrows(); i++) {
        // Get group of current row
        String group = allGroups[i];
        // Get word of current row
        String word = allWords[i];
        // Get count of current row
        int count = allCounts[i];
        // Add count to current group's count
        groupCount[group] += count;
        // Add count to current word's count
        wordCount[word] += count;
        // Add count to totalCount
        totalCount += count;
        // Add count of group and word combo to counts
        counts[group][word] = count;
    }
}

// Get all unique groups and words
void getGroupsWords() {
    // Get all unique words
    words = text["word"];
    words = unique(words);
    // Loop through words
    for (int i = 0; i < words.size(); i++) {
        // Add all words to map with count of 0
        wordCount[words[i]] = 0;
    }
    // Get all unique groups
    groups = text["group"];
    groups = unique(groups);
    // Loop through groups
    for (int i = 0; i < groups.size(); i++) {
        // Add all groups to map with count of 0
        groupCount[groups[i]] = 0;
    }
}

// Function to compute log likelihood
// Input a dataframe with columns for word, group, and n (word count in group)
// Outputs a dataframe with a column for all unique words
// Subsequent columns contain LL scores for a given word for all unique groups
// [[Rcpp::export]]
DataFrame log_likelihood(DataFrame t) {
    // Assign value to text private variable
    text = t;

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