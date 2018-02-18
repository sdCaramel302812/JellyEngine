#include "ResourceManager.h"


string ResourceManager::_file_name;
string ResourceManager::_path_name;

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}


void ResourceManager::loadResource(char * fileName)
{
	FILE *inFile;
	fopen_s(&inFile, fileName, "r");
	char content[512];
	char *buf;
	string target;
	fread(content, 512, 1, inFile);
	
	char *word;
	
	word = strtok_s(content, "< =>\n\t", &buf);
	while (word != NULL) {
		if (strcmp(word, "Path")) {
			target = "Path";
		}
		if (strcmp(word, "/Path")) {
			_path_name = "";
		}
		if (strcmp(word, "Image")) {
			target = "Image";
		}
		if (strncmp(word, "\"", 1) == 0) {
			string name;
			string key;
			for (char *c = word + 1; *c != '"'; ++c) {
				if (*c != '"') {
					name.push_back(*c);
				}
			}
			for (char *c = word + 1; *c != '"'; ++c) {
				if (*c == '.' && target == "Image") {
					break;
				}
				if (*c != '"') {
					key.push_back(*c);
				}
			}
			if (target == "Path") {
				_path_name = name;
				
			}
			if (target == "Image") {
				_file_name = _path_name;
				_file_name.append(name);
				
				Render::addTexture(_file_name, key);
			}
		}

		word = strtok_s(NULL, "< =>\n\t", &buf);
	}

	fclose(inFile);
}
