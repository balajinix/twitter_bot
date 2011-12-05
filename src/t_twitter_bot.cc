#include <iostream>
#include "twitter_bot.h"

int main(int argc, char* argv[]) {

  if (argc < 5 || argc > 6) {
    std::cout << "Usage: " << argv[0] << " <user_name> <password> <test_type, 0-query, 1-post, 2-reply> <query/message> [to_user]\n";
    return -1;
  }
  return 0;

}

