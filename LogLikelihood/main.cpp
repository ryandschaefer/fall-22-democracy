#include <iostream>
#include <fstream>
#include "LogLikelihood_class.h"
using namespace std;

vector<vector<string>> input();
void output(const map<string, vector<double>>&);
void outputText(const vector<vector<string>>&);
vector<vector<string>> clean(const vector<string>&);

// For testing purposes
int main() {
    // Create log likelihood object
    LogLikelihood ll;
    // Read input file and clean data
    vector<vector<string>> data = input();

    outputText(data);

    // Call log likelihood function
    map<string, vector<double>> results = ll.getLogLikelihood(data);
    // Call output function
    output(results);
    return 0;
}

// Read input file
vector<vector<string>> input() {
    // Create input stream
    ifstream in("testing.txt");
    // Read input file
    vector<string> lines;
    while (!in.eof()) {
        string temp;
        getline(in, temp);
        if (temp.length() > 0) {
            lines.push_back(temp);
        } else {
            break;
        }
    }
    return clean(lines);
}

// Split, lowercase, and remove punctuation
vector<vector<string>> clean(const vector<string>& text) {
    vector<vector<string>> cleaned;
    // Loop through groups
    for (const string& i : text) {
        // Vector to hold split text of group
        vector<string> curr;
        char* temp = new char[i.length()];
        strcpy(temp, i.c_str());
        char* token = strtok(temp, " .,';/:-)(");
        while (token != nullptr) {
            string str = token;
            for (char& j : str) {
                j = tolower(j);
            }
            curr.push_back(str);
            token = strtok(nullptr, " .,';/:-");
        }
        cleaned.push_back(curr);
    }
    return cleaned;
}

// Create output file
void output(const map<string, vector<double>>& results) {
    // Create output stream
    ofstream out("results.csv");
    // Loop through words in result
    for (const auto& itr : results) {
        // Output word
        out << itr.first;
        // Loop through LL scores
        for (const double& val : itr.second) {
            // Output LL score
            out << "," << val;
        }
        // New line
        out << endl;
    }
    // Close output file
    out.close();
}

void outputText(const vector<vector<string>>& text) {
    ofstream out("text.txt");
    for (int i = 0; i < text.size(); i++) {
        out << text[i][0];
        for (int j = 1; j < text[i].size(); j++) {
            out << "," << text[i][j];
        }
        out << endl;
    }
    out.close();
}