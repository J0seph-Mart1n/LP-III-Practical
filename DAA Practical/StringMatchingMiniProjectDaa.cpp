#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

vector<int> naiveStringMatching(const string& text, const string& pattern) {
    int n = text.length();
    int m = pattern.length();
    vector<int> matches;

    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j])
                break;
        }
        if (j == m)
            matches.push_back(i);
    }

    return matches;
}

vector<int> rabinKarp(const string& text, const string& pattern, int prime = 101) {
    int n = text.length();
    int m = pattern.length();
    vector<int> matches;
    int base = 256; // Number of characters in the input alphabet
    int patternHash = 0, textHash = 0, h = 1;

    // Calculate h = base^(m-1) % prime
    for (int i = 0; i < m - 1; i++)
        h = (h * base) % prime;

    // Calculate the hash value for pattern and the first window of text
    for (int i = 0; i < m; i++) {
        patternHash = (base * patternHash + pattern[i]) % prime;
        textHash = (base * textHash + text[i]) % prime;
    }

    // Slide the pattern over text one by one
    for (int i = 0; i <= n - m; i++) {
        // Check if the hash values match
        if (patternHash == textHash) {
            // If the hash values match, check the characters one by one
            if (text.substr(i, m) == pattern)
                matches.push_back(i);
        }

        // Calculate hash value for the next window of text
        if (i < n - m) {
            textHash = (base * (textHash - text[i] * h) + text[i + m]) % prime;

            // Make sure textHash is positive
            if (textHash < 0)
                textHash += prime;
        }
    }

    return matches;
}

void measureNaiveTime(const string &text, const string &pattern, int iterations) {
    // Measuring time for Naive String Matching
    auto startNaive = chrono::high_resolution_clock::now();
    vector<int> naiveMatches;
    for (int i = 0; i < iterations; i++)
        naiveMatches = naiveStringMatching(text, pattern);
    auto endNaive = chrono::high_resolution_clock::now();
    chrono::duration<double> durationNaive = (endNaive - startNaive)/iterations;

    cout << "Naive String Matching Algorithm:\n";
    cout << "Execution time: " << durationNaive.count() << " seconds\n";
    cout << endl;
}

void measureRabinKarpTime(const string &text, const string &pattern, int iterations, int prime = 101) {
    // Measuring time for Rabin-Karp Algorithm
    auto startRabinKarp = chrono::high_resolution_clock::now();
    vector<int> rabinKarpMatches;
    for (int i = 0; i < iterations; i++)
        rabinKarpMatches = rabinKarp(text, pattern, prime);
    auto endRabinKarp = chrono::high_resolution_clock::now();
    chrono::duration<double> durationRabinKarp = endRabinKarp - startRabinKarp;

    cout << "Rabin-Karp Algorithm (with prime = " << prime << "):\n";
    cout << "Execution time: " << durationRabinKarp.count() << " seconds\n";
    cout << endl;
}

int main() {
    string text1 = "THIS IS AN EXAMPLE.";
    string pattern1 = "EX";

    string text2 = "";
    for (int i = 0; i < 1000; i++) {
        text2 += "THIS IS A SIMPLE EXAMPLE ";
    }
    string pattern2 = "EXAMPLE";

    string text3 = "ABABABACABABABABACABABABACABABABAC";
    string pattern3 = "ABABABACABABABABAC";
    
    int iterations = 1000;

    cout << "Text1, Pattern1 = " << pattern1 << " Iterations = 1000" << endl;
    measureNaiveTime(text1, pattern1, iterations);
    measureRabinKarpTime(text1, pattern1, iterations);
    measureRabinKarpTime(text1, pattern1, iterations, 211);
    measureRabinKarpTime(text1, pattern1, iterations, 307);
    cout << endl;


    cout << "Text2, Pattern2 = " << pattern1 << " Iterations = 1000" << endl;
    measureNaiveTime(text2, pattern2, iterations);
    measureRabinKarpTime(text2, pattern2, iterations);
    measureRabinKarpTime(text2, pattern2, iterations, 211);
    measureRabinKarpTime(text2, pattern2, iterations, 307);
    cout << endl;

    cout << "Text3, Pattern3 = " << pattern1 << " Iterations = 1000" << endl;    
    measureNaiveTime(text3, pattern3, iterations);
    measureRabinKarpTime(text3, pattern3, iterations);
    measureRabinKarpTime(text3, pattern3, iterations, 211);
    measureRabinKarpTime(text3, pattern3, iterations, 307);
    cout << endl;
    return 0;
}