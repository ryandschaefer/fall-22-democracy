#include <Rcpp.h>
#include <map>
using namespace Rcpp;
using namespace std;

// Variables
// Input dataframe
DataFrame Text;
// Wanted group names
String Group1;
String Group2;
// List of words to calculate JSD for
CharacterVector WordList;
// String variables with input column names
String Group;
String Word;
String Count;
// Dataframe to hold word probabilities
map<String, pair<double, double>> wordProbs;
// Map to hold final results
DataFrame results;

// Functions
// Jensen-Shannon Divergence
double calcJSD(double, double);
// Kullback-Leibler Divergence
double KLD(double, double);
// Joint Entropy
double H(double, double);
// Entropy
double H(double);
// Calculate word probabilites
void getProbabilites();
// Calculate JSD
void getJSD();


// Exported function
// [[Rcpp::export]]
DataFrame jsd(DataFrame text, String group1 = "", String group2 = "", CharacterVector word_list = CharacterVector::create(), String group = "group", String word = "word", String n = "n") {
  // Set private variables to input values
  Group1 = group1;
  Group2 = group2;
  Text = text;
  Group = group;
  Word = word;
  Count = n;
  if (word_list.size() == 0) {
    // If word_list is empty, use all words
    CharacterVector allWords = text[Word];
    WordList = unique(allWords);
  } else {
    // Else, use given words
    WordList = word_list;
  }
  
  // Get unique groups
  CharacterVector groups = Text[Group];
  groups = unique(groups);
  // Set groups 1 & 2 if not given by user
  if (Group1 == "") {
    Group1 = groups[0];
  }
  if (Group2 == "") {
    Group2 = groups[1];
  }
  
  getProbabilites();
  getJSD();
  
  return results;
}

// Jensen-Shannon Divergence
double calcJSD(double p, double q) {
  // If p = 0 or q = 0, return 0
  if (p == 0.0 || q == 0.0) {
    return 0;
  }
  // Find midpoint
  double r = (p + q) / 2;
  // Calculate JSD
  return (KLD(p, r) + KLD(q, r)) / 2;
}

// Kullback-Leibler Divergence
double KLD(double p, double q) {
   // Return joint entropy - entropy(P)
   return H(p, q) - H(p);
}

// Joint Entropy
double H(double x, double y) {
  // Return joint probability * log(joint robability)
  double jointProb = x * y;
  return jointProb * log(jointProb);
}

// Entropy
double H(double x) {
  // Return probability * log(probability)
  return x * log(x);
}

// Calculate word probabilites
void getProbabilites() {
  // Initialize ints for group counts
  int count1 = 0;
  int count2 = 0;
  
  // Get vectors for group, word, and count
  CharacterVector allGroups = Text[Group];
  CharacterVector allWords = Text[Word];
  NumericVector allCounts = Text[Count];
  
  // Loop through Text dataframe
  for (int i = 0; i < Text.nrows(); i++) {
    // Only include word in WordList
    if (find(WordList.begin(), WordList.end(), allWords[i]) != WordList.end()) {
      // Check if row group is either Group1 or Group2
      if (allGroups[i] == Group1) {
        if (wordProbs.find(allWords[i]) == wordProbs.end()) {
          // Create new value in map if word not there yet
          pair<double, double> temp;
          wordProbs[allWords[i]] = temp;
        }
        // Set word count in group
        wordProbs[allWords[i]].first = allCounts[i];
      } else if (allGroups[i] == Group2) {
        if (wordProbs.find(allWords[i]) == wordProbs.end()) {
          // Create new value in map if word not there yet
          pair<double, double> temp;
          wordProbs[allWords[i]] = temp;
        }
        // Set word count in group
        wordProbs[allWords[i]].second = allCounts[i];
      }
    }
    // Update group counts
    if (allGroups[i] == Group1) {
      // Update group 1 count
      count1 += allCounts[i];
    } else if (allGroups[i] == Group2) {
      // Update group 2 count
      count2 += allCounts[i];
    }
  }
  
  // Loop through wordProbs
  for (auto& itr : wordProbs) {
    // Divide each count by total count for that group
    itr.second.first /= count1;
    itr.second.second /= count2;
  }
}

// Calculate JSD
void getJSD() {
  // Initialize vectors
  CharacterVector words;
  NumericVector scores;
  
  // Loop through wordProbs
  for (const auto& itr : wordProbs) {
    // Get JSD of current word
    double jsd = calcJSD(itr.second.first, itr.second.second);
    // Append word and jsd to vectors
    words.push_back(itr.first);
    scores.push_back(jsd);
  }
  
  // Create results dataframe from vectors
  results = DataFrame::create(Named(Word) = words, Named("JSD") = scores);
}
