#include <Rcpp.h>
#include <map>
using namespace Rcpp;
using namespace std;

// Variables
// Input dataframe
DataFrame text;
// Wanted group names
String group1;
String group2;
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
DataFrame JSD(DataFrame t, String g1 = "", String g2 = "", String group = "group", String word = "word", String n = "n") {
  // Set private variables to input values
  group1 = g1;
  group2 = g2;
  text = t;
  Group = group;
  Word = word;
  Count = n;
  
  // Get unique groups
  CharacterVector groups = text[Group];
  groups = unique(groups);
  // Set groups 1 & 2 if not given by user
  if (group1 == "") {
    group1 = groups[0];
  }
  if (group2 == "") {
    group2 = groups[1];
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
  CharacterVector allGroups = text[Group];
  CharacterVector allWords = text[Word];
  NumericVector allCounts = text[Count];
  
  // Loop through text dataframe
  for (int i = 0; i < text.nrows(); i++) {
    // Check if row group is either group1 or group2
    if (allGroups[i] == group1) {
      // Increment total group count
      count1 += allCounts[i];
      // Set word count in group
      if (wordProbs.find(allWords[i]) == wordProbs.end()) {
        // Create new value in map if word not there yet
        pair<double, double> temp;
        wordProbs[allWords[i]] = temp;
      }
      wordProbs[allWords[i]].first = allCounts[i];
    } else if (allGroups[i] == group2) {
      count2 += allCounts[i];
      // Set word count in group
      if (wordProbs.find(allWords[i]) == wordProbs.end()) {
        // Create new value in map if word not there yet
        pair<double, double> temp;
        wordProbs[allWords[i]] = temp;
      }
      wordProbs[allWords[i]].second = allCounts[i];
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
