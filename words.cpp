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


    std::vector<words::word_frequency> topk;

    for (int i = 0; i < k; i++) {
      topk.push_back(dist.at(i));
    }

    // std::vector<words::word_frequency>::iterator minit; 

    // for (auto x :dist){
    //   for (minit = topk.begin(); minit != topk.end();){
    //     if (minit->count() < x.count()){
    //       *minit = x;
    //     }
    //     ++minit;
    //   }
    // }
    std::vector<words::word_frequency>::iterator minfreq = topk.begin();
    std::vector<words::word_frequency>::iterator tmp;

    for (int i = 0; i < dist.size(); i++) {
      // minfreq = std::min_element(topk.begin(), topk.end(),
      //   [&] (const words::word_frequency& left, const words::word_frequency& right) 
      //   {return left.count() < right.count();}
      //   );
      if (dist.at(i).count() > minfreq->count()) {
        *minfreq = dist.at(i);
        for (tmp = topk.begin(); tmp != topk.end();) {
          if (minfreq->count() > tmp->count()){
            minfreq = tmp;
          }
          tmp++;
        }
        // for (int j = 0; j < topk.size(); j++) {
        //   if (minfreq->count() > topk.at(j).count()) {
        //     *minfreq = topk.at(j);
        //   }
        // }
      }
    }

    // std::vector<std::pair<std::string,int>> topk2; 

    // std::vector<std::pair<std::string,int>> dist2;

    // for (int i = 0; i < dist.size(); i++){

    //   dist2[i].first = dist[i].word();
    //   dist2[i].second = dist[i].count();

    // }

    // sort(dist2.begin(), dist2.end(), [&](std::pair<std::string, int> &left, std::pair<std::string, int> &right){ 
    //   return left.second> right.second;
    // } );


    // for (int i = 0; i < k; i++) {
    //   topk2.push_back(dist2[i]);
    // }

    // for (auto x : topk2){
    //   topk.push_back(words::word_frequency(x.first, x.second));
    // }

    // for (int i = 0; i < k; i++) {
    //   topk.push_back(dist.at(i));      
    // }
    
    // int min_ele; 

    // for (auto x : dist){
    //   min_ele = std::min_element(topk.begin(), topk.end(),  )
    // }

    // int i=0;
    // for (auto x : dist) {
    //   if (i++ < k) {
    //     topk.push_back(x);
    //   }
    //   else if (x.count() > topk.begin()->count()) {
    //     topk.erase(topk.begin());
    //     topk.push_back(x);
    //   }
    // }
    

    
    // TODO: Write a definition for this function that actually
    // works. That will include deleting the current return statement,
    // which is just a placeholder to allow the code to compile. When
    // you are done, delete this TODO comment.

    return topk;
  }

  frequency_distribution
  vector_word_counter::count(
    const word_vector& stop_words,
    const word_vector& document
  ) const noexcept {

    // TODO: Write a definition for this function that actually
    // works. That will include deleting the current return statement,
    // which is just a placeholder to allow the code to compile. When
    // you are done, delete this TODO comment.

    std::vector<std::string> stopWords;    // Initialize vector of stop words
    stopWords = stop_words;              // Copy stop words into vector
    std::vector<words::word_frequency> frequencyDistribution;     // Initialize vector of word frequencies

    std::vector<words::word_frequency>::iterator it;     // Initialize iterator for word frequencies

    for (int i = 0; i < document.size(); i++) {
      if (std::find(stopWords.begin(), stopWords.end(), document.at(i)) == stopWords.end()) {     // Check if word is stop word

        // bool isEqual(const words::word_frequency& word) {
        //   return word.word() == dictionary.at(i);
        // }
        // it = std::find_if(frequencyDistribution.begin(), frequencyDistribution.end(), isEqual());

       //it = find(frequencyDistribution.begin()->word(), frequencyDistribution.end()->word(), document.at(i)); // Find index of word
       it = std::find_if(frequencyDistribution.begin(), frequencyDistribution.end(), 
        [&](const words::word_frequency& word) {return word.word() == document.at(i);});
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

    // TODO: Write a definition for this function that actually
    // works. That will include deleting the current return statement,
    // which is just a placeholder to allow the code to compile. When
    // you are done, delete this TODO comment.

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
      dists.push_back(wf);
    }

    return dists;
  } 

  frequency_distribution
  hash_word_counter::count(
    const word_vector& stop_words,
    const word_vector& document
  ) const noexcept {

    // TODO: Write a definition for this function that actually
    // works. That will include deleting the current return statement,
    // which is just a placeholder to allow the code to compile. When
    // you are done, delete this TODO comment.

    std::unordered_set<std::string> stopWords;      // Initialize hash table of stop words
    for (int i = 0; i < stop_words.size(); i++) {
      stopWords.insert(stop_words.at(i));       // Populate hash table with stop words 
    }

    std::unordered_map<std::string, int> frequencyDistribution;     // Initalize hash table of word frequencies
    //std::unordered_set<words::word_frequency> frequencyDistribution; 

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

    // TODO: Write a definition for this function that actually
    // works. That will include deleting the current return statement,
    // which is just a placeholder to allow the code to compile. When
    // you are done, delete this TODO comment.
    
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
