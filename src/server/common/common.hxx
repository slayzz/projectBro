#ifndef WEBSOCKET_COMMON_HXX
#define WEBSOCKET_COMMON_HXX

#include <vector>
#include <string>
namespace Common {

  typedef std::pair<std::string, std::string> FilePair;

  std::vector<std::string> split(const std::string& parsed, std::string&& delim);
  FilePair getFilePair(std::string filename);

}

#endif //WEBSOCKET_COMMON_HXX
