#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<iostream>
#include<sstream>

class FileManager
{
public:
	FileManager(void);
	~FileManager(void);

	void LoadContent(const char* filename, 
		std::vector<std::vector<std::string>> &attributes,
		std::vector<std::vector<std::string>> &contents);

	void LoadContent(const char* filename, 
		std::vector<std::vector<std::string>> &attributes,
		std::vector<std::vector<std::string>> &contents, std::string id);
protected:
private:
	std::vector<std::string> tempAttributes;
	std::vector<std::string> tempContents;

	bool idFound;

};

#endif