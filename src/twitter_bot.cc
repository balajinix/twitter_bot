#include "twitter_bot.h"
#include <iostream>
#include <fstream>
#include <string>
#include "JSONValue.h"

#ifdef DEBUG
#if DEBUG>0
#define TWITTERBOT_DEBUG DEBUG
#endif
#endif
//#define TWITTERBOT_DEBUG 1

namespace balajinix {

TwitterBot::TwitterBot() {
#ifdef TWITTERBOT_DEBUG
  m_debug_level = TWITTERBOT_DEBUG;
#endif // TWITTERBOT_DEBUG
}

TwitterBot::~TwitterBot() {
  Clear();
}

int TwitterBot::SetDebugLevel(unsigned int debug_level) {
  m_debug_level = debug_level;
}

int TwitterBot::InitTwitterClient(std::string& user_name, std::string& pass_word) {

  if (user_name.empty() || pass_word.empty()) {
#ifdef TWITTERBOT_DEBUG
    std::cerr << "ERROR: invalid input\n";
#endif // TWITTERBOT_DEBUG
    return -1;
  }

  std::ifstream ifs("../data/search_max_ids.txt");
  if (ifs.is_open()) {
    std::string temp_url;
    std::string last_search_max_id;
    while (getline(ifs, temp_url)) {
      if (getline(ifs, last_search_max_id))
        m_search_data_map[temp_url] = last_search_max_id;
    }
  }
  ifs.close();

#ifndef TWITTERBOT_DEBUG
  std::string tmpStr( "" );
  std::string replyMsg( "" );

  /* OAuth flow begins */
  /* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
  m_twitterObj.getOAuth().setConsumerKey( std::string( "vlC5S1NCMHHg8mD1ghPRkA" ) );
  m_twitterObj.getOAuth().setConsumerSecret( std::string( "3w4cIrHyI3IYUZW5O2ppcFXmsACDaENzFdLIKmEU84" ) );

  /* Step 1: Check if we alredy have OAuth access token from a previous run */
  char szKey[1024];
  std::string myOAuthAccessTokenKey("");
  std::string myOAuthAccessTokenSecret("");
  std::ifstream oAuthTokenKeyIn;
  std::ifstream oAuthTokenSecretIn;

  oAuthTokenKeyIn.open( "../data/token_key.txt" );
  oAuthTokenSecretIn.open( "../data/token_secret.txt" );

  memset( szKey, 0, 1024 );
  oAuthTokenKeyIn >> szKey;
  myOAuthAccessTokenKey = szKey;

  memset( szKey, 0, 1024 );
  oAuthTokenSecretIn >> szKey;
  myOAuthAccessTokenSecret = szKey;

  oAuthTokenKeyIn.close();
  oAuthTokenSecretIn.close();
  
  if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
  {
    /* If we already have these keys, then no need to go through auth again */
    //printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );

    m_twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
    m_twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
  }
  else
  {
    /* Step 2: Get request token key and secret */
    m_twitterObj.oAuthRequestToken(tmpStr);

    /* Step 3: Ask user to visit web link and get PIN */
    // char szOAuthVerifierPin[1024];
    // memset( szOAuthVerifierPin, 0, 1024 );
    printf( "\nPlease visit this link in web browser and authorize this application:\n%s", tmpStr.c_str() );
    printf( "\nEnter the PIN provided by twitter: " );
    getline(std::cin, tmpStr);
    //tmpStr = szOAuthVerifierPin;
    m_twitterObj.getOAuth().setOAuthPin( tmpStr );

    /* Step 4: Exchange request token with access token */
    m_twitterObj.oAuthAccessToken();

    /* Step 5: Now, save this access token key and secret for future use without PIN */
    m_twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
    m_twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

    /* Step 6: Save these keys in a file or wherever */
    std::ofstream oAuthTokenKeyOut;
    std::ofstream oAuthTokenSecretOut;

    oAuthTokenKeyOut.open( "../data/token_key.txt" );
    oAuthTokenSecretOut.open( "../data/token_secret.txt" );

    oAuthTokenKeyOut.clear();
    oAuthTokenSecretOut.clear();

    oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
    oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

    oAuthTokenKeyOut.close();
    oAuthTokenSecretOut.close();
  }
  /* OAuth flow ends */

  /* Set twitter username and password */
  m_twitterObj.setTwitterUsername(user_name);
  m_twitterObj.setTwitterPassword(pass_word);

/*
  // Set proxy server usename, password, IP and port (if present)
  char proxyPresent[1024];
  memset( proxyPresent, 0, 1024 );

  printf( "\nDo you have a proxy server configured (0 for no; 1 for yes): " );
  gets( proxyPresent );
  int isProxy = atoi( proxyPresent );

  if( isProxy > 0 )
  {
    char proxyIp[1024];
    char proxyPort[1024];
    char proxyUsername[1024];
    char proxyPassword[1024];

    memset( proxyIp, 0, 1024 );
    memset( proxyPort, 0, 1024 );
    memset( proxyUsername, 0, 1024 );
    memset( proxyPassword, 0, 1024 );

    printf( "\nEnter proxy server IP: " );
    gets( proxyIp );
    printf( "\nEnter proxy server port: " );
    gets( proxyPort );
    printf( "\nEnter proxy server username: " );
    gets( proxyUsername );
    printf( "\nEnter proxy server password: " );
    gets( proxyPassword );

    tmpStr = proxyIp;
    m_twitterObj.setProxyServerIp( tmpStr );
    tmpStr = proxyPort;
    m_twitterObj.setProxyServerPort( tmpStr );
    tmpStr = proxyUsername;
    m_twitterObj.setProxyUserName( tmpStr );
    tmpStr = proxyPassword;
    m_twitterObj.setProxyPassword( tmpStr );
  }
*/
#endif // TWITTERBOT_DEBUG

  return 0;
}

int TwitterBot::LoadMapFromFile(std::string& file_name, std::map<std::string, std::string>& string_map) {

  std::ifstream ifs(file_name.c_str());
  if (!ifs) {
    std::cerr << "ERROR: could not open file " << file_name << std::endl;
    return -1;
  }

  std::string entry; 
  std::string line;
  std::string value;
  std::string::size_type loc = 0;
  int num_docs = 0;
  while (getline(ifs, line)) {
    num_docs++;
    if ((loc = line.find("=", 0)) != std::string::npos) {
      entry = std::string(line, 0, loc);
      value = std::string(line, loc+1, line.length() - loc); 
      string_map[entry] = value; 
    } else {
      std::cerr << "ERROR: malformed corpus entry on line number " << num_docs \
                << " in " << file_name << std::endl;
      ifs.close();
      return -1;
    }
  }
  ifs.close();

  return num_docs;
}

int TwitterBot::LoadMultiMapFromFile(std::string& file_name,
                                   std::multimap<std::string, std::string>& multi_map) {

  std::ifstream ifs(file_name.c_str());
  if (!ifs) {
    std::cerr << "ERROR: could not open file " << file_name << std::endl;
    return -1;
  }

  std::string entry; 
  std::string line;
  std::string value;
  std::string::size_type loc = 0;
  int num_docs = 0;
  while (getline(ifs, line)) {
    num_docs++;
    if ((loc = line.find("=", 0)) != std::string::npos) {
      entry = std::string(line, 0, loc);
      value = std::string(line, loc+1, line.length() - loc); 
      multi_map.insert(std::pair<std::string, std::string> (entry, value));; 
    } else {
      std::cerr << "ERROR: malformed corpus entry on line number " << num_docs \
                << " in " << file_name << std::endl;
      ifs.close();
      return -1;
    }
  }
  ifs.close();

  return num_docs;
}

int TwitterBot::WriteMapToFile(std::map<std::string, std::string>& string_map, const std::string& file_name) {

  std::ofstream ofs(file_name.c_str());
  if (!ofs) {
    std::cerr << "ERROR: could not open file " << file_name << std::endl;
    return -1;
  }

  std::map<std::string, std::string>::iterator map_iter;
  for (map_iter = string_map.begin(); map_iter != string_map.end(); map_iter++) {
    ofs << (*map_iter).first << "=" << (*map_iter).second << std::endl;
  }
  ofs.close();

  return 0;
}

int TwitterBot::TwitterSearchUrl(const std::string& url,
                                 std::map<std::string, std::string> &tweets_map) {

  std::string temp_url = url;
  std::string last_search_max_id = m_search_data_map[url];
  if (last_search_max_id.size() > 0) {
    temp_url += "&since_id=" + last_search_max_id;
  }

#ifdef TWITTERBOT_DEBUG
  if (m_debug_level > 2) {
    std::cout << temp_url << std::endl;
  }
#endif // TWITTERBOT_DEBUG

  int num_docs = 0;
  bool ret_value = false;
  twitCurl m_twitterObj;

  ret_value = m_twitterObj.getTweets(temp_url.c_str());
  if (!ret_value) {
    std::cerr << "ERROR: couldn't get tweets" << std::endl;
    return -1;
  }

  std::string reply_message;
  m_twitterObj.getLastWebResponse(reply_message);
  if (reply_message.size() <= 0) {
    m_twitterObj.getLastCurlError(reply_message);
    std::cerr << "Twitter Search Error: " << reply_message << std::endl;
    return -1;
  }

  JSONValue *json_value = JSON::Parse(reply_message.c_str());
  if (!json_value || (false == json_value->IsObject())) {
    std::cerr << "ERROR: JSON::Parse failed for query: " << temp_url << std::endl;
  } else {
#ifdef TWITTERBOT_DEBUG
    if (m_debug_level > 2) {
      std::cout << reply_message << std::endl;
    }
#endif // TWITTERBOT_DEBUG
    std::string script;
    std::string tweet;
    std::string tweet_id;
    std::string tweeter;
    int count = 0;
    std::ofstream ofs("../data/tweet_id_tweet_map.txt");
    JSONObject tweet_o = json_value->AsObject();
    if (tweet_o.find("results") != tweet_o.end() && tweet_o["results"]->IsArray()) {
      JSONArray tweet_array = tweet_o["results"]->AsArray();
      for (unsigned int i=0; i < tweet_array.size(); i++) {
        JSONValue *tweet_value = tweet_array[i];
        if (false == tweet_value->IsObject()) {
          std::cerr << "ERROR: tweet_value is not an object" << std::endl;
        } else {
          if (count == 0) {
            tweet.clear();
            tweeter = "unknown";
            tweet_id.clear();
          }
          JSONObject tweet_object = tweet_value->AsObject();
          if (tweet_object.find("text") != tweet_object.end() && tweet_object["text"]->IsString()) {
            tweet = tweet_object["text"]->AsString(); 
            if (tweet.find("http") != std::string::npos) {
              count = 0;
              continue;
            }
            count++;
          }
          if (tweet_object.find("from_user") != tweet_object.end() && tweet_object["from_user"]->IsString()) {
            tweeter = tweet_object["from_user"]->AsString();
            count++;
          }
          if (tweet_object.find("id_str") != tweet_object.end() && tweet_object["id_str"]->IsString()) {
            tweet_id = tweet_object["id_str"]->AsString(); 
            count++;
          }
          if (count == 3) {
            ofs << tweet_id << "=" << tweet << std::endl;
            tweets_map.insert(std::pair<std::string, std::string> (tweeter, tweet_id));
            ++num_docs;
            count = 0;
#ifdef TWITTERBOT_DEBUG
            if (m_debug_level > 0) {
              std::cout << tweet_id << "= @" << tweeter << " " << tweet << std::endl;
            }
#endif // TWITTERBOT_DEBUG
          }
        }
      }
    }
    ofs.close();

    if (tweet_o.find("max_id_str") != tweet_o.end() && tweet_o["max_id_str"]->IsString()) {
      last_search_max_id = tweet_o["max_id_str"]->AsString();
    } else {
      std::cerr << "max id field not found in twitter response\n";
    }
    if (last_search_max_id.size() <= 0) {
      std::cerr << "max id value is empty\n";
    } else {
      m_search_data_map[url] = last_search_max_id;
    }
  }
  delete json_value;

  return num_docs;
}

int TwitterBot::TwitterSearch(const std::string& query,
                            std::map<std::string, std::string>& tweets) {
  std::string url = "http://search.twitter.com/search.json?q='" + query + "'&rpp=10";
  int ret_value = 0;
  if ((ret_value = TwitterSearchUrl(url, tweets)) < 0) {
    std::cerr << "Error: could not get tweets for query: " << query << std::endl;
  }
  return ret_value;
}

int TwitterBot::Post(std::string& statusMsg) {

  /* Post a new status message */
  std::string replyMsg = "";
  if (m_twitterObj.statusUpdate(statusMsg)) {
    m_twitterObj.getLastWebResponse(replyMsg);
    //printf( "\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
  } else {
    m_twitterObj.getLastCurlError(replyMsg);
    printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
    return -1;
  }

  usleep(30000000);

  return 1;
}

int TwitterBot::Post(std::string& statusMsg, std::string& reply_to_id) {

  /* Post a new status message */
  std::string replyMsg = "";
  if (m_twitterObj.postReply(statusMsg, reply_to_id)) {
    m_twitterObj.getLastWebResponse(replyMsg);
    // printf( "\ntwitterClient:: twitCurl::postReply web response:\n%s\n", replyMsg.c_str() );
  } else {
    m_twitterObj.getLastCurlError(replyMsg);
    printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
    return -1;
  }

  usleep(30000000);

  return 1;
}

int TwitterBot::PostFromFile(std::string& handles_file,
                           std::string& class_file,
                           std::string& messages_file) {

  std::map<std::string, std::string> handles_map;
  std::map<std::string, std::string>::iterator handles_iter;
  if (LoadMapFromFile(handles_file, handles_map) < 0) {
    std::cerr << "ERROR: could not load handles file" << std::endl;
    return -1;
  }

  std::map<std::string, std::string> classes_map;
  std::map<std::string, std::string>::iterator class_iter;
  if (LoadMapFromFile(class_file, classes_map) < 0) {
    std::cerr << "ERROR: could not load class file" << std::endl;
    return -1;
  }

  std::multimap<std::string, std::string> messages_map;
  std::multimap<std::string, std::string>::iterator messages_iter;
  std::pair<std::multimap<std::string, std::string>::iterator,
            std::multimap<std::string, std::string>::iterator> messages; 
  if (LoadMultiMapFromFile(messages_file, messages_map) < 0) {
    std::cerr << "ERROR: could not load messages file" << std::endl;
    return -1;
  }

  std::string message;
  std::string text_class;
  std::string status;
  std::string tweet_id;
  int num_messages = 0;
  int ret_value = 0;
  int num_posted = 0;
  for (handles_iter = handles_map.begin(); handles_iter != handles_map.end(); handles_iter++) {
    tweet_id = handles_iter->second;
    if ((class_iter = classes_map.find(handles_iter->second)) == classes_map.end()) {
      std::cerr << "ERROR: cannot find class for tweet_id: " << handles_iter->second << std::endl;
      continue;
    }
    message.clear();
    messages = messages_map.equal_range(class_iter->second);
    num_messages = 0;
    for (messages_iter = messages.first; messages_iter != messages.second; messages_iter++) {
      num_messages++;
    }
    //int i = rand() % messages.size();
#ifdef TWITTERBOT_DEBUG
    if (m_debug_level > 2) {
      std::cout << "num_messages: " << num_messages << " for class: " << class_iter->second << std::endl;
    }
#endif // TWITTERBOT_DEBUG
    int i = rand() % num_messages;
    int j = 0;
    for (messages_iter = messages.first; messages_iter != messages.second; messages_iter++) {
      if (j == i) {
        message = messages_iter->second;
        break;
      }
      j++;
    }
    if (message.empty()) {
     std::cerr << "ERROR: invalid message\n";
     continue;
    }
    status = "@" + handles_iter->first + " " + message;
#ifndef TWITTERBOT_DEBUG
    if ((ret_value = Post(status, tweet_id)) < 0) {
      std::cerr << "ERROR: could not post: " << status << " in reply to: " << tweet_id << std::endl;
    } else {
      num_posted += ret_value;
    }
#else
    std::cout << status << " in reply to " << tweet_id << std::endl;
#endif // TWITTERBOT_DEBUG
  }

  handles_map.clear();
  classes_map.clear();
  messages_map.clear();

  return num_posted;
}

int TwitterBot::Clear() {

  if (m_search_data_map.empty())
    return 0;
  std::ofstream ofs("../data/search_max_ids.txt");
  if (ofs.is_open()) {
    std::map<std::string, std::string>::iterator map_iter;
    for (map_iter = m_search_data_map.begin(); map_iter != m_search_data_map.end(); map_iter++) {
      ofs << map_iter->first << std::endl;
      ofs << map_iter->second << std::endl;
    }
  }
  ofs.close();
  m_search_data_map.clear();

  return 0;
}

int TwitterBot::FindTargets(std::string& handles_master_file, // input file
                            std::string& class_name, // input
                            std::map<std::string, std::string>& tweets, // input handle to tweet_id map
                            std::string& handles_to_spam_file, // output
                            std::string& tweet_id_class_name_file // output
                           ) {

  std::map<std::string, std::string> handles_map;
  if (LoadMapFromFile(handles_master_file, handles_map) < 0) {
    std::cerr << "ERROR: could not load handles_master_map from file: " << handles_master_file << std::endl;
    return -1;
  }

  std::ofstream ofs1(handles_to_spam_file.c_str());
  std::ofstream ofs2(tweet_id_class_name_file.c_str());

  std::string handle;
  std::string tweet_id;
  std::map<std::string, std::string>::iterator map_iter;
  unsigned int count = 0;
  for (map_iter=tweets.begin(); map_iter != tweets.end(); map_iter++) {
    handle = map_iter->first;
    tweet_id = map_iter->second;

    if (handles_map.find(handle) != handles_map.end()) {
#ifdef TWITTERBOT_DEBUG
      if (m_debug_level > 3) {
        std::cout << "handle: " << handle << " already in handles_master. updating" << std::endl;
      }
#endif // TWITTERBOT_DEBUG
      handles_map[handle] = tweet_id;
    } else {
      ofs1 << handle << "=" << tweet_id << std::endl;
      handles_map[handle] = tweet_id;
      ofs2 << tweet_id << "=" << class_name << std::endl;
      count++;
    }
  }

  ofs1.close();
  ofs2.close();

  handles_map.clear();

  return count;
}

} // namespace
