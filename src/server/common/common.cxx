#include "common.hxx"

namespace Common {

  std::vector<std::string> split(const std::string& parsed, std::string&& delim) {
    std::vector<std::string> splits;

    size_t currentPosition = 0, newPosition = 0;
    do {
      newPosition = parsed.find(delim, currentPosition);
      auto difference = (newPosition - currentPosition);
      splits.push_back(parsed.substr(currentPosition,
                                     difference > parsed.length() ?
                                     parsed.length() :
                                     (newPosition - currentPosition)));
      currentPosition = newPosition + 1;
    } while(newPosition != std::string::npos);

    return splits;
  }


  FilePair getFilePair(std::string filename) {
    auto position = filename.find(".");
    return FilePair(filename.substr(0, position),
                    filename.substr(position + 1, filename.length()));
  };

}

