#include "ResourceManager.h"


string ResourceManager::_file_name;
string ResourceManager::_path_name;
vector<string> ResourceManager::_image_list;

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}


void ResourceManager::loadResource(char * fileName)
{
	FILE *inFile = nullptr;
	fopen_s(&inFile, fileName, "r");
	char content[512];
	memset(content, 0, sizeof(content));
	char *buf;
	string target;
	size_t r = 0;
	fread(content, 1, 511, inFile);
	//content[511] = 0;
	
	char *word;
	
	word = strtok_s(content, "< =>\n\t", &buf);
	++r;
	while (word != NULL) {
		if (strcmp(word, "Path") == 0) {
			target = "Path";
		}
		if (strcmp(word, "/Path") == 0) {
			_path_name = "";
		}
		if (strcmp(word, "Image") == 0) {
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
				
				_image_list.push_back(key);
				Render::addTexture(_file_name, key);
			}
		}
		++r;
		word = strtok_s(NULL, "< =>\n\t", &buf);
	}

	fclose(inFile);
}

void ResourceManager::saveMap(char * fileName)
{
	int tab_num = 0;
}

vector<string>& ResourceManager::getImageList()
{
	return _image_list;
}
