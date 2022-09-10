#ifndef LOGLIKELIHOOD_LOGLIKELIHOOD_CLASS_H
#define LOGLIKELIHOOD_LOGLIKELIHOOD_CLASS_H

#include <map>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

class LogLikelihood {
public:
    // Constructor
    LogLikelihood() = default;
    // Launch function
    map<string, vector<double>> getLogLikelihood(vector<vector<string>>&);
private:
    // Private variables
    // 2d vector to hold words in each group
    vector<vector<string>> text;
    // Map to hold final results
    map<string, vector<double>> results;
    // Map to contain counts of each word in each group
    map<string, vector<int>> counts;
    // Vector to count the number of words in each group
    vector<int> groupCount;
    // Map to hold the total number of times each word appears
    map<string, int> wordCount;
    // Total word count
    int totalCount;

    // Private functions

    // Counting words
    void getCounts();
    void getGroupCounts();
    void getWordCounts();
    int countGroup(const int&);
    int countWord(const string&);

    // Calculating log likelihood
    void calcLL();
    double calcLL(const string&, const int&);
};

#endif //LOGLIKELIHOOD_LOGLIKELIHOOD_CLASS_H
