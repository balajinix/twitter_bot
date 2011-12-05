#include "twitter_bot.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <set>

int main(int argc, char* argv[]) {

  if (argc < 7 || argc > 8) {
    std::cout << "Usage: \t" << argv[0] \
              << "\n\t<0/1/2 - test_type, 0-search,1-tweets by user/2-mentions on user/3-url> " \
              << "\n\t<input_value - query/handle/url>]" \
              << "\n\t<class_name>" \
              << "\n\t<handles_master_file>" \
              << "\n\t<handles_to_spam_file>" \
              << "\n\t<tweet_id_class_name_file>" \
              << "\n\t<debug_level>\n";
    return -1;
  }

  unsigned int input_type = atoi(argv[1]);

  std::string input_value;
  input_value = std::string(argv[2]);
  if (input_value.empty()) {
    std::cerr << "ERROR: input_value needed\n";
    return -1;
  }

  std::string class_name;
  class_name = std::string(argv[3]);
  if (class_name.empty()) {
    std::cerr << "ERROR: class_name needed\n";
    return -1;
  }

  std::string handles_master_file;
  handles_master_file = std::string(argv[4]);
  if (input_value.empty()) {
    std::cerr << "ERROR: input_value needed\n";
    return -1;
  }
  std::string handles_to_spam_file = "../data/handles_to_spam.txt";
  std::string tweet_id_class_name_file = "../data/tweet_id_class_name_map.txt";

  unsigned int debug_level = 0;
  if (argc == 8) {
    debug_level = atoi(argv[7]);
  }

  std::map<std::string, std::string> tweets;
  std::map<std::string, std::string>::iterator map_iter;
  std::string url;

  balajinix::TwitterBot bot;
  bot.SetDebugLevel(debug_level);

  switch (input_type) {
    case 0:
      url = std::string("http://search.twitter.com/search.json?q=" + input_value/* + "&rpp=100"*/);
      if (bot.TwitterSearchUrl(url, tweets) < 0) {
        std::cerr << "ERROR: No results found\n";
      }
      break;
    case 1:
      url = std::string("http://search.twitter.com/search.json?q=from:" + input_value/* + "&rpp=100"*/);
      if (bot.TwitterSearchUrl(url, tweets) < 0) {
        std::cerr << "ERROR: twitter searcher failed\n";
      }
      break;
    case 2:
      url = std::string("http://search.twitter.com/search.json?q=to\%3A" + input_value);
      if (bot.TwitterSearchUrl(url, tweets) < 0) {
        std::cerr << "ERROR: twitter searcher failed\n";
      }
      break;
    case 3:
      {
        // std::string query("madras\%20OR\%20chennai\%20OR\%20election\%20OR\%20vote\%20OR\%20politics&geocode=12.984722,80.251944,40mi");
        url = input_value;
        if (bot.TwitterSearchUrl(url, tweets) < 0) {
          std::cerr << "ERROR: twitter searcher failed\n";
        }
      }
      break;
    default:
      break;
  }

  if (tweets.empty()) {
    std::cerr << "ERROR: No tweets found from search\n";
    return 0;
  }

  int num_targets = 0;
  if ((num_targets = bot.FindTargets(handles_master_file, // input file
                      class_name, // input
                      tweets, // input handle to tweet_id map
                      handles_to_spam_file, // output
                      tweet_id_class_name_file // output
                     )) < 0) {
    std::cerr << "ERROR: could not find targets\n";
  } else {
    std::cout << num_targets << " targets found\n";
  }

  tweets.clear();

  return 0;
}
