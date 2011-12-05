#include <string>
#include <map>
#include <set>
#include "twitcurl.h"

namespace balajinix {

class TwitterBot {
 public:
  TwitterBot();
  ~TwitterBot();
  int SetDebugLevel(unsigned int debug_level);
  int Clear();
  int TwitterSearchUrl(const std::string& url,
                       std::map<std::string, std::string>& tweets_map);
  int TwitterSearch(const std::string& query,
                    std::map<std::string, std::string>& tweets_map);
  int LoadMapFromFile(std::string& file_name,
                      std::map<std::string, std::string>& string_map);
  int LoadMultiMapFromFile(std::string& file_name,
                           std::multimap<std::string, std::string>& multi_map);

  int WriteMapToFile(std::map<std::string, std::string>& string_map,
                     const std::string& file_name);
  int InitTwitterClient(std::string& user_name, std::string& pass_word);
  int Post(std::string& statusMsg);
  int Post(std::string& statusMsg, std::string& reply_to_id);
  int PostFromFile(std::string& handles_file,
                   std::string& class_file,
                   std::string& messages_file);
  int FindTargets(std::string& handles_master_file, // input file
                  std::string& message_tag, // input
                  std::map<std::string, std::string>& tweets, // input handle to tweet_id map
                  std::string& handles_to_spam_file, // output
                  std::string& tweet_id_message_tag_file // output
                 );

 private:
  twitCurl m_twitterObj;
  std::map<std::string, std::string> m_search_data_map;
  unsigned int m_debug_level;
};

}
