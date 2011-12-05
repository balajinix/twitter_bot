#include <cstdio>
#include <iostream>
#include <fstream>
#include "twitter_bot.h"

#ifdef DEBUG
#if DEBUG>0
#define TWITTERBOT_DEBUG DEBUG
#endif
#endif
//#define TWITTERBOT_DEBUG 1

int main( int argc, char* argv[]){

  if (argc < 7 || argc > 8) {
    std::cout << "Usage: " << argv[0] << "\n\t <user_name>\n\t <password>\n\t <handle_master_file>\n\t <handles_file>\n\t <class_file>\n\t <messages_file>\n\t <debug_level>\n";
    return -1;
  }

  std::string user_name = std::string(argv[1]);
  std::string pass_word = std::string(argv[2]);
  std::string handles_master_file = std::string(argv[3]);
  std::string handles_file = std::string(argv[4]);
  std::string classes_file = std::string(argv[5]);
  std::string messages_file = std::string(argv[6]);
  unsigned int debug_level = 0;
  if (argc == 8) {
    debug_level = atoi(argv[7]);
  }

  balajinix::TwitterBot bot;

  if (argc == 8) {
    bot.SetDebugLevel(debug_level);
  }

  bot.InitTwitterClient(user_name, pass_word);

  std::map<std::string, std::string> current_handles_map;
  std::map<std::string, std::string>::iterator map_iter;
  bot.LoadMapFromFile(handles_file, current_handles_map);

  std::map<std::string, std::string> handles_map;
  bot.LoadMapFromFile(handles_master_file, handles_map);

  std::string handle;
  bool flag = true;
  for (map_iter=current_handles_map.begin(); map_iter != current_handles_map.end(); map_iter++) {
#ifdef TWITTERBOT_DEBUG
    if (debug_level > 1) {
      std::cout << map_iter->first << "=" << map_iter->second << std::endl;
    }
#endif // TWITTERBOT_DEBUG
    handle = map_iter->first;
    if (handles_map.find(handle) != handles_map.end()) {
      std::cerr << "ERROR: trying to spam the same user again: " << handle << std::endl;
      flag = false;
    } else {
      handles_map[handle] = map_iter->second;
    }
  }
  current_handles_map.clear();

  bot.WriteMapToFile(handles_map, handles_master_file);
  handles_map.clear();

  if (!flag) {
    std::cerr << "Unresolved error: Not spamming\n";
  } else {
    int num_spammed = 0;
    if ((num_spammed = bot.PostFromFile(handles_file, classes_file, messages_file)) < 0) {
      std::cerr << "ERROR: could not post from file\n";
    } else {
      std::cout << num_spammed << " spam messages posted\n";
    }
  }

  bot.Clear();

  return 0;
}
