#include "StringOperations.hpp"

StringOperations::StringOperations()
{
	//ctor
}

StringOperations::~StringOperations()
{
	//dtor
}
std::string StringOperations::readFile(const std::string & path){
	const char * fileName = path.c_str();

    std::ifstream file ( fileName, std::ios::in);
    std::string fileContent;
    if(file.is_open()) {
        std::string line = "";
        while(getline(file, line)) { 	// read per line
            fileContent += line + "\n";		// add to string
        }
        file.close(); // close on finish
    } else {
        std::cout << "Error reading from " << fileName << std::endl;
        fileContent = "";
    }
    return fileContent;

}


std::vector<std::string>& StringOperations::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> StringOperations::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}
 void StringOperations::myReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != std::string::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}
