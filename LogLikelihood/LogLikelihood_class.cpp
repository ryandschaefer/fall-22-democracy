#include "LogLikelihood_class.h"

// Function to compute log likelihood
// 2d vector represents the text:
    // First layer is each group
    // Second layer is each word in that group
// Returns map containing:
    // Every unique word as a key
    // The log likelihood score for that word for each group in the value
map<string, vector<double>> LogLikelihood::getLogLikelihood(vector<vector<string>>& t) {
    // Clear old results
    results.clear();
    // Assign value to text private variable
    text = t;

    // Count how many times each word appears in each group
    getCounts();
    // Calculate log likelihood scores
    calcLL();

    // Clear all private variables (other than result)
    text.clear();
    counts.clear();
    groupCount.clear();
    wordCount.clear();

    // Return result
    return results;
}

// Function to count how many times each word appears in each group
void LogLikelihood::getCounts() {
    // Loop through groups
    for (int i = 0; i < text.size(); i++) {
        // Loop through words
        for (const string& word : text[i]) {
            // Check if the word is in counts
            if (counts.find(word) == counts.end()) {
                // If not found:
                // Create initial vector for word with all 0s
                vector<int> c(text.size(), 0);
                // Update value in counts vector corresponding to group
                c[i] = 1;
                // Add word to counts map with initial vector
                counts[word] = c;
            } else {
                // If found:
                // Increment correct value in word's count vector
                counts[word][i]++;
            }
        }
    }
    // Call functions to get group and word counts
    getGroupCounts();
    getWordCounts();
    // Get total word count by taking the sum of groupCount
    totalCount = 0;
    for (int curr : groupCount) {
        totalCount += curr;
    }
}

// Count the number of words in each group
void LogLikelihood::getGroupCounts() {
    // Loop through group indices
    for (int i = 0; i < text.size(); i++) {
        // Append value of countGroup for current group to groupCount
        groupCount.push_back(countGroup(i));
    }
}

// Count the number of occurrences of each word
void LogLikelihood::getWordCounts() {
    // Loop through counts map
    for (const auto& itr : counts) {
        // Append value of countWord for current word to wordCount
        wordCount[itr.first] = countWord(itr.first);
    }
}

// Count the number of words in the group with the given index
int LogLikelihood::countGroup(const int& index) {
    // Counter variable
    int cnt = 0;
    // Loop through counts map
    for (const auto& itr : counts) {
        // Increment counter by correct value from iterator vector
        cnt += itr.second[index];
    }
    // Return the count
    return cnt;
}

// Count the number of occurrences of the given word
int LogLikelihood::countWord(const string& word) {
    // Vector to hold the counts for the given word from all groups
    vector<int> curr = counts[word];
    // Counter variable
    int cnt = 0;
    // Loop through curr
    for (int c : curr) {
        // Increment count by c
        cnt += c;
    }
    // Return the count
    return cnt;
}

// Calculate the log likelihood score for each word in each group
void LogLikelihood::calcLL() {
    // Iterate through counts map
    for (const auto& itr : counts) {
        // Create vector of doubles to hold LL scores for this word
        vector<double> ll_scores;
        // Loop through each group
        for (int i = 0; i < text.size(); i++) {
            // Append LL score for current word and group
            ll_scores.push_back(calcLL(itr.first, i));
        }
        // Add ll_scores to results map
        results[itr.first] = ll_scores;
    }
}

// Calculate the log likelihood score for one word in one group
double LogLikelihood::calcLL(const string& word, const int& index) {
    // Word count in group
    int a = counts[word][index];
    // Word count in remaining groups
    int b = wordCount[word] - a;
    // Remaining word count in group
    int c = groupCount[index] - a;
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
