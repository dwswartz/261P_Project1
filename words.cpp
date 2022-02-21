///////////////////////////////////////////////////////////////////////////////
// words.cpp
//
// Computing the frequency distribution of a list of words, skipping stopwords.
//
// Students: you do not need to modify this file.
///////////////////////////////////////////////////////////////////////////////

// instructor's includes, delete from starter code
#include <algorithm>
#include <cctype>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "words.hpp"

namespace words {

  word_vector read_words(const std::string& path) {
    word_vector result;
    std::ifstream file(path);
    if (!file) {
      throw io_exception(path);
    }
    for (std::string word; file >> word;) {
      result.emplace_back(std::move(word));
    }
    file.close();
    if (file.bad()) {
      throw io_exception(path);
    }
    return result;
  }

  std::string lowercase(const std::string& str) {
    auto result(str);
    std::for_each(result.begin(), result.end(),
                  [](char& c) { c = tolower(c); });
    return result;
  }

  /////////////////////////////////////////////////////////////////////////////
  // START OF CODE TO MODIFY
  /////////////////////////////////////////////////////////////////////////////

  frequency_distribution
  most_frequent(
    const frequency_distribution& dist,
    size_t k
  ) noexcept {

    // std::vector<words::word_frequency> tempdist;
    // std::vector<words::word_frequency> topk;
    // tempdist = dist;

    // std::sort(tempdist.rbegin(), tempdist.rend(),         
    // [&](const words::word_frequency& word1, const words::word_frequency& word2)
    // { return (word1.count() < word2.count());} );     // sort the vector of word_frequency by count 
    //                                                   // and reverse the iterators to get descending order

    // for (int i = 0; i < k; i++){    
    //   topk.push_back(tempdist.at(i));                 // get top k elements 
    // }

    std::vector<words::word_frequency> tempdist;
    std::vector<words::word_frequency> topk;
    tempdist = dist;
    std::vector<words::word_frequency>::iterator max_ele;

    for (int i = 0; i < k; i++){
      max_ele = std::max_element(tempdist.begin(), tempdist.end(),
        [&](const words::word_frequency& word1, const words::word_frequency& word2) {
          return (word1.count() < word2.count());} );
      topk.push_back(*max_ele);
      tempdist.erase(max_ele);    
    }

    return topk;
  }

  frequency_distribution
  vector_word_counter::count(
    const word_vector& stop_words,
    const word_vector& document
  ) const noexcept {

    std::vector<std::string> stopWords;    // Initialize vector of stop words
    stopWords = stop_words;              // Copy stop words into vector
    std::vector<words::word_frequency> frequencyDistribution;     // Initialize vector of word frequencies

    std::vector<words::word_frequency>::iterator it;     // Initialize iterator for word frequencies

    for (int i = 0; i < document.size(); i++) {
      if (std::find(stopWords.begin(), stopWords.end(), document.at(i)) == stopWords.end()) {     // Check if word is stop word
       it = std::find_if(frequencyDistribution.begin(), frequencyDistribution.end(), 
        [&](const words::word_frequency& word) {return word.word() == document.at(i);});    // iterator to find if word is in the distribution vector
        if (it != frequencyDistribution.end()) {        // Word exists in frequency table
          it->increment_count();          // Increment frequency of second element of pair 
        }   
        else{
          frequencyDistribution.push_back(words::word_frequency(document.at(i), 1));    // Add word to word frequencies with 1 count 
        }
      }
    }
    return frequencyDistribution;
  }

  frequency_distribution
  list_word_counter::count(
    const word_vector& stop_words,
    const word_vector& document
  ) const noexcept {

    std::list<std::string> stopWords;       // Initialize linked list of stop words
    std::list<words::word_frequency> frequencyDistribution;     // Initialize linked list of word frequencies

    for (int i = 0; i < stop_words.size(); i++) {
      stopWords.push_back(stop_words.at(i));        // Populate stopWords linked list from vector
    }

    std::list<words::word_frequency>::iterator it;     // Initialize iterator 

    for (int i = 0; i < document.size(); i++) {
      if (std::find(stopWords.begin(), stopWords.end(), document.at(i)) == stopWords.end()) {   // Check if word is stop word
        it = std::find_if(frequencyDistribution.begin(), frequencyDistribution.end(), 
        [&](const words::word_frequency& word) {return word.word() == document.at(i);});    // Find index of word

        if (it != frequencyDistribution.end()) {        // Word exists in frequency table
          it->increment_count();          // Increment frequency 
        }
        else {
          frequencyDistribution.push_back(words::word_frequency(document.at(i), 1));    // Add word to word frequencies with 1 count 
        }
      }
    }

    std::vector<words::word_frequency> dists;     // Initialize vector of word frequencies

    for (words::word_frequency &wf: frequencyDistribution) {
      dists.push_back(wf);              // convert list to vector
    }

    return dists;
  } 

  frequency_distribution
  hash_word_counter::count(
    const word_vector& stop_words,
    const word_vector& document
  ) const noexcept {

    std::unordered_set<std::string> stopWords;      // Initialize hash table of stop words
    for (int i = 0; i < stop_words.size(); i++) {
      stopWords.insert(stop_words.at(i));       // Populate hash table with stop words 
    }

    std::unordered_map<std::string, int> frequencyDistribution;     // Initalize hash table of word frequencies

    for (int i = 0; i < document.size(); i++) {       // Iterate through document 
      if (stopWords.find(document.at(i)) == stopWords.end()) {      // Check if word is stop word 
        if (frequencyDistribution.find(document.at(i)) != frequencyDistribution.end()) {
          frequencyDistribution[document.at(i)]++;       // Increment frequency by 1 
        }
        else {
          frequencyDistribution.insert({document.at(i), 1});    // Add word to frequency distribution
        }
      }
    }

    std::vector<words::word_frequency> dists;     // Initialize vector of word frequencies

    for (auto it:frequencyDistribution) {
      dists.push_back(words::word_frequency(it.first, it.second));         // Populate vector with word frequencies
    }

    return dists;
  }

  frequency_distribution
  tree_word_counter::count(
    const word_vector& stop_words,
    const word_vector& document
  ) const noexcept {

    std::set<std::string> stopWords; // initialize stop words

    for (int i = 0; i < stop_words.size(); i++) {
      stopWords.insert(stop_words.at(i));
    }
   
   std::map<std::string, int> frequencyDistribution;

   for (int i = 0; i < document.size(); i++) {       // Iterate through document 
      if (stopWords.find(document.at(i)) == stopWords.end()) {      // Check if word is stop word 
        if (frequencyDistribution.find(document.at(i)) != frequencyDistribution.end()) {
          frequencyDistribution[document.at(i)]++;       // Increment frequency by 1 
        }
        else {
          frequencyDistribution.insert(std::make_pair(document.at(i), 1));    // Add word to frequency distribution
        }
      }
    }
    
    std::vector<words::word_frequency> dists;     // Initialize vector of word frequencies

    for (auto it:frequencyDistribution) {
      dists.push_back(words::word_frequency(it.first, it.second));         // Populate vector with word frequencies
    }

    return dists;
  }

  /////////////////////////////////////////////////////////////////////////////
  // END OF CODE TO MODIFY
  /////////////////////////////////////////////////////////////////////////////

}
