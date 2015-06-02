#include "FileManager.h"

FileManager::FileManager(void)
{

}

FileManager::~FileManager(void)
{

}

enum LoadState { ATTRIBUTES, CONTENTS };

int state;

void FileManager::LoadContent(const char* filename, 
		std::vector<std::vector<std::string>> &attributes,
		std::vector<std::vector<std::string>> &contents)
{
	attributes.clear();
	contents.clear();

	std::ifstream openfile(filename);
	if(openfile.is_open())
	{
		while(!openfile.eof())
		{
			std::string line;

			std::getline(openfile, line);

			line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

			if(line.find("Load=") != std::string::npos)
			{
				state = ATTRIBUTES;
				line.erase(0, line.find('=') + 1);
				tempAttributes.clear();
			}
			else
			{
				state = CONTENTS;
				tempContents.clear();
			}

			line.erase(std::remove(line.begin(), line.end(), '['), line.end());

			std::stringstream str(line);

			while(str)
			{
				std::getline(str, line, ']');
				if(line != "")
				{
					if(state == ATTRIBUTES)
					{
						tempAttributes.push_back(line);
					}
					else
					{
						tempContents.push_back(line);
						std::cout << line << std::endl;
					}
				}
			}

			if(state == CONTENTS && tempContents.size() > 0)
			{
				attributes.push_back(tempAttributes);
				contents.push_back(tempContents);
			}
		}
	}
	else
	{

	}
}

void FileManager::LoadContent(const char* filename, 
		std::vector<std::vector<std::string>> &attributes,
		std::vector<std::vector<std::string>> &contents, std::string id)
{
	attributes.clear();
	contents.clear();

	idFound = false;

	std::ifstream openfile(filename);
	if(openfile.is_open())
	{
		while(!openfile.eof())
		{
			std::string line;

			std::getline(openfile, line);

			line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

			if(line.find("Start=[" + id +"]") != std::string::npos)
			{
				idFound = true;
				continue;
			}
			else if(line.find("End=") != std::string::npos && line.find(id) != std::string::npos)
			{
				idFound = false;
				break;
			}

			if(idFound)
			{

				if(line.find("Load=") != std::string::npos)
				{
					state = ATTRIBUTES;
					line.erase(0, line.find('=') + 1);
					tempAttributes.clear();
				}
				else
				{
					state = CONTENTS;
					tempContents.clear();
				}

				line.erase(std::remove(line.begin(), line.end(), '['), line.end());

				std::stringstream str(line);

				while(str)
				{
					std::getline(str, line, ']');
					if(line != "")
					{
						if(state == ATTRIBUTES)
						{
							tempAttributes.push_back(line);
						}
						else
						{
							tempContents.push_back(line);
							std::cout << line << std::endl;
						}
					}
				}

				if(state == CONTENTS && tempContents.size() > 0)
				{
					attributes.push_back(tempAttributes);
					contents.push_back(tempContents);
				}
			}
		}
	}
	else
	{

	}
}