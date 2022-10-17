#include <Rcpp.h>
#include <map>
#include <cstring>
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
double kld(double, double);
double KLD(vector<double>&, vector<double>&);
// Joint Entropy
double h(double, double);
double H(vector<double>&, vector<double>&);
// Entropy
double h(double);
double H(vector<double>&);
// Calculate word probabilites
void getProbabilites();
// Get JSD of one group pairing
void jsdPair();
// Get cumulative JSD of one group pairing
void jsdPairCumulative();
// Initialize wordProbs
void setWordProbs();
// Clear private variables
void clearVars();

// Exported function
// [[Rcpp::export]]
DataFrame jsd(DataFrame text, bool cumulative = false, CharacterVector group_list = CharacterVector::create(), CharacterVector word_list = CharacterVector::create(), String group = "group", String word = "word", String n = "n") {
  // Set private variables to input values
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
  
  // Initialize wordProbs
  setWordProbs();
  
  // If not enough groups given, pick for user
  if (group_list.size() < 2) {
    // Get all unique groups
    CharacterVector allGroups = Text[Group];
    allGroups = unique(allGroups);
    if (group_list.size() == 1) {
      // If 1 group was given, set Group1 to given group
      Group1 = group_list[0];
      if (allGroups[0] == Group1) {
        // If first group is Group1, set Group2 to second group
        Group2 = allGroups[1];
      } else {
        // Else, set Group2 to first group
        Group2 = allGroups[0];
      }
    } else {
      // Else, use first two groups
      Group1 = allGroups[0];
      Group2 = allGroups[1];
    }
    
    if (cumulative) {
      // If cumulative, return results of cumulative JSD for selected pair
      jsdPairCumulative();
    } else {
      // Else, return results of JSD for selected pair
      jsdPair();
    }
    
    // Clear private variables
    clearVars();
    
    // Return results
    return results;
  }
  
  // Initialze output data frame
  DataFrame output = DataFrame::create();
  // Loop through given groups
  for (int i = 0; i < group_list.size(); i++) {
    // Loop through groups after current group
    for (int j = i + 1; j < group_list.size(); j++) {
      String g1 = group_list[i];
      String g2 = group_list[j];
      /// Set groups 1 and 2 to current groups
      Group1 = group_list[i];
      Group2 = group_list[j];
      if (cumulative) {
        // If cumulative, calculate cumulative JSD score for group pairing
        // Calculate JSD scores for current group pairing
        jsdPairCumulative();
        // Store JSD scores
        CharacterVector names = results.names();
        String name = names[0];
        output.push_back(results[0], name.get_cstring());
      } else {
        // Else, calculate JSD scores for group pairing
        // Calculate JSD scores for current group pairing
        jsdPair();
        // Store JSD scores
        CharacterVector names = results.names();
        String name = names[1];
        output.push_back(results[1], name.get_cstring());
      }
      
    }
  }
  if (!cumulative) {
    // If not cumulative, add word column to output
    output.push_front(results["word"], "word");
  }
  
  // Clear private variables
  clearVars();
  
  return output;
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
  return (kld(p, r) + kld(q, r)) / 2;
}

// Kullback-Leibler Divergence
double kld(double p, double q) {
   // Return joint entropy - entropy(P)
   double val = h(p, q) - h(p);
   return val;
}

// Kullback-Leibler Divergence
double KLD(vector<double>& P, vector<double>& Q) {
  // Return joint entropy - entropy(P)
  double val = H(P, Q) - H(P);
  return val;
}

// Joint Entropy
double h(double x, double y) {
  // Return joint probability * log(joint robability)
  double jointProb = x * y;
  return jointProb * log(jointProb);
}

// Joint Entropy
double H(vector<double>& X, vector<double>& Y) {
  double sum = 0;
  // Loop through distributions
  for (const auto& x : X) {
    for (const auto& y : Y) {
      // Calculate joint probability
      double jointProb = x * y;
      // Add product of joint probability and log(joint probability) to sum
      sum += jointProb * log(jointProb);
    }
  }
  // Make sum negative and return it
  sum *= -1;
  return sum;
}

// Entropy
double h(double x) {
  // Return probability * log(probability)
  return x * log(x);
}

// Entropy
double H(vector<double>& X) {
  double sum = 0;
  // Loop through distribution
  for (const auto& x : X) {
    // Add product of probability and log(probability) to sum
    sum += x * log(x);
  }
  // Make sum negative and return
  sum *= -1;
  return sum;
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
        // Set word count in group
        wordProbs[allWords[i]].first = allCounts[i];
      } else if (allGroups[i] == Group2) {
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

// Calculate the JSD score for a group pairing
void jsdPair() {
  // Calculate word probabilities
  getProbabilites();
  
  // Initialize vectors
  CharacterVector words;
  NumericVector scores;
  
  // Loop through wordProbs
  for (const auto& itr : wordProbs) {
    if (itr.second.first != 0 && itr.second.second != 0) {
      // If both probabilities are nonzero, get JSD of current word
      double jsd = calcJSD(itr.second.first, itr.second.second);
      // Append word and jsd to vectors
      words.push_back(itr.first);
      scores.push_back(jsd);
    }
  }
  
  // Use concatenated group names as column name
  char* temp = new char[strlen(Group1.get_cstring()) + strlen(Group2.get_cstring()) + 1];
  strcpy(temp, Group1.get_cstring());
  strcat(temp, "_");
  strcat(temp, Group2.get_cstring());
  string name = temp;
  delete[] temp;
  
  // Create results dataframe from vectors
  results = DataFrame::create(Named(Word) = words, Named(name) = scores);
}

// Get cumulative JSD of one group pairing
void jsdPairCumulative() {
  // Calculate word probabilities
  getProbabilites();

  // Vectors to hold probability distributions
  vector<double> P;
  vector<double> Q;
  vector<double> R;

  // Loop through word probabilities
  for (const auto& itr : wordProbs) {
    if (itr.second.first != 0 && itr.second.second != 0) {
      // If both probabilites are not 0, add probabilities to distributions
      P.push_back(itr.second.first);
      Q.push_back(itr.second.second);
      // Find midpoint
      double mid = 0.5 * (itr.second.first + itr.second.second);
      R.push_back(mid);
    }
  }

  // Calculate JSD
  double jsd = (KLD(P, R) + KLD(Q, R)) / 2;

  // Create NumericVector and add jsd
  NumericVector col = NumericVector::create(jsd);

  // Use concatenated group names as column name
  char* temp = new char[strlen(Group1.get_cstring()) + strlen(Group2.get_cstring()) + 1];
  strcpy(temp, Group1.get_cstring());
  strcat(temp, "_");
  strcat(temp, Group2.get_cstring());
  string name = temp;
  delete[] temp;

  // Create results data frame from col
  results = DataFrame::create(Named(name) = col);
}

// Initialize wordProbs
void setWordProbs() {
  // Loop through WordList
  for (int i = 0; i < WordList.size(); i++) {
    // Add pair to wordProbs for current word
    pair<double, double> temp;
    wordProbs[WordList[i]] = temp;
  }
}

// Clear private variables
void clearVars() {
  Text = DataFrame::create();
  Group1 = "";
  Group2 = "";
  WordList = CharacterVector::create();
  Group = "";
  Word = "";
  Count = "";
  wordProbs.clear();
}
