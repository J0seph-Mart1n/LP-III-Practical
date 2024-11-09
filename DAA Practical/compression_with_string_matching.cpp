#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

using namespace std;

// Function to find all repeating patterns in the text using Naive String Matching
vector<pair<string, int>> findPatternsNaive(const string& text, int minLength) {
    vector<pair<string, int>> patterns;
    unordered_map<string, int> patternCount;
    int n = text.length();

    // Search for patterns of length >= minLength
    for (int length = minLength; length <= n / 2; length++) {
        for (int i = 0; i <= n - length; i++) {
            string pattern = text.substr(i, length);
            patternCount[pattern]++;
        }
    }

    // Collect patterns that repeat
    for (const auto& entry : patternCount) {
        if (entry.second > 1) {
            patterns.push_back(entry);
        }
    }

    return patterns;
}

// Function to find all repeating patterns in the text using Rabin-Karp
vector<pair<string, int>> findPatternsRabinKarp(const string& text, int minLength, int prime = 101) {
    vector<pair<string, int>> patterns;
    unordered_map<string, int> patternCount;
    int n = text.length();
    int base = 256; // Number of characters in the input alphabet

    for (int length = minLength; length <= n / 2; length++) {
        int patternHash = 0, textHash = 0, h = 1;

        // Calculate h = base^(length-1) % prime
        for (int i = 0; i < length - 1; i++) {
            h = (h * base) % prime;
        }

        // Calculate the hash value for the first window of text
        for (int i = 0; i < length; i++) {
            patternHash = (base * patternHash + text[i]) % prime;
            textHash = (base * textHash + text[i]) % prime;
        }

        // Slide the pattern over text one by one
        for (int i = 0; i <= n - length; i++) {
            // Check if the hash values match
            if (patternHash == textHash) {
                string pattern = text.substr(i, length);
                patternCount[pattern]++;
            }

            // Calculate hash value for the next window of text
            if (i < n - length) {
                textHash = (base * (textHash - text[i] * h) + text[i + length]) % prime;

                // Make sure textHash is positive
                if (textHash < 0) {
                    textHash += prime;
                }
            }
        }
    }

    // Collect patterns that repeat
    for (const auto& entry : patternCount) {
        if (entry.second > 1) {
            patterns.push_back(entry);
        }
    }

    return patterns;
}

// Function to compress the text using identified patterns
string compressText(const string& text, unordered_map<string, string>& encoding) {
    string compressedText = text;
    for (const auto& pair : encoding) {
        size_t pos = compressedText.find(pair.first);
        while (pos != string::npos) {
            compressedText.replace(pos, pair.first.length(), pair.second);
            pos = compressedText.find(pair.first, pos + pair.second.length());
        }
    }
    return compressedText;
}

// Function to write the encoding dictionary to a file
void writeEncodingDictionary(const unordered_map<string, string>& encoding, const string& filename) {
    ofstream outFile(filename);
    for (const auto& pair : encoding) {
        outFile << pair.first << " -> " << pair.second << endl;
    }
    outFile.close();
}

long getFileSize(const string& filename) {
    ifstream inFile(filename, ios::ate | ios::binary);
    return inFile.tellg();
}


int main() {
    // Read text from a file
    ifstream inputFile("input.txt");
    stringstream buffer;
    buffer << inputFile.rdbuf();
    string text = buffer.str();
    inputFile.close();

        // Display original file size
    long originalSize = text.size();
    cout << "Original file size: " << originalSize << " bytes\n" << endl;

    // Find repeating patterns using Naive String Matching
    auto startNaive = chrono::high_resolution_clock::now();
    int minLength = 3; // Minimum length of patterns to look for
    vector<pair<string, int>> patternsNaive = findPatternsNaive(text, minLength);
    auto endNaive = chrono::high_resolution_clock::now();
    chrono::duration<double> durationNaive = endNaive - startNaive;

    // Create a dictionary for encoding patterns using Naive
    unordered_map<string, string> encodingNaive;
    int patternIndex = 1;
    for (const auto& pattern : patternsNaive) {
        string encodedValue = "P" + to_string(patternIndex++);
        encodingNaive[pattern.first] = encodedValue;
    }

    // Compress the text using the encoding dictionary from Naive
    string compressedTextNaive = compressText(text, encodingNaive);
    long compressedSizeNaive = compressedTextNaive.size();

    // Write the encoding dictionary to a file
    writeEncodingDictionary(encodingNaive, "encoding_naive.txt");

    // Output the results
    ofstream outputFileNaive("compressed_naive.txt");
    outputFileNaive << compressedTextNaive;
    outputFileNaive.close();

    cout << "Naive String Matching:\n";
    cout << "Time taken: " << durationNaive.count() << " seconds\n";
    cout << "Compressed text written to 'compressed_naive.txt'\n";
    cout << "Compressed file size (Naive): " << compressedSizeNaive << " bytes" << endl;
    cout << "Encoding dictionary written to 'encoding_naive.txt'\n\n";

    // Find repeating patterns using Rabin-Karp
    auto startRabinKarp = chrono::high_resolution_clock::now();
    vector<pair<string, int>> patternsRabinKarp = findPatternsRabinKarp(text, minLength);
    auto endRabinKarp = chrono::high_resolution_clock::now();
    chrono::duration<double> durationRabinKarp = endRabinKarp - startRabinKarp;

    // Create a dictionary for encoding patterns using Rabin-Karp
    unordered_map<string, string> encodingRabinKarp;
    patternIndex = 1;
    for (const auto& pattern : patternsRabinKarp) {
        string encodedValue = "P" + to_string(patternIndex++);
        encodingRabinKarp[pattern.first] = encodedValue;
    }

    // Compress the text using the encoding dictionary from Rabin-Karp
    string compressedTextRabinKarp = compressText(text, encodingRabinKarp);
    long compressedSizeRabinKarp = compressedTextRabinKarp.size();

    // Write the encoding dictionary to a file
    writeEncodingDictionary(encodingRabinKarp, "encoding_rabin_karp.txt");

    // Output the results
    ofstream outputFileRabinKarp("compressed_rabin_karp.txt");
    outputFileRabinKarp << compressedTextRabinKarp;
    outputFileRabinKarp.close();

    cout << "Rabin-Karp Algorithm:\n";
    cout << "Time taken: " << durationRabinKarp.count() << " seconds\n";
    cout << "Compressed text written to 'compressed_rabin_karp.txt'\n";
    cout << "Compressed file size (Rabin-Karp): " << compressedSizeRabinKarp << " bytes" << endl;
    cout << "Encoding dictionary written to 'encoding_rabin_karp.txt'\n";

    return 0;
}
