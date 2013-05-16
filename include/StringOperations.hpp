#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

class StringOperations
{
	public:
		StringOperations();
		virtual ~StringOperations();

		static std::string readFile( const std::string& filePath );

		static std::vector<std::string> split(const std::string &s, char delim);
		static std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
		static void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr);

	protected:
	private:
};

#endif // STRINGOPERATIONS_H
