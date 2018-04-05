#include "ResourceManager.h"


string ResourceManager::_file_name;
string ResourceManager::_path_name;
vector<string> ResourceManager::_image_list;
vector<string> ResourceManager::_map_list;

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

void ResourceManager::loadMapList(char * dirName)
{
	HANDLE hnd;
	WIN32_FIND_DATA FindFileData;
	string fileName = dirName;
	char target[64];
	sprintf_s(target, "%s%s", dirName, "*.level");
	if ((hnd = FindFirstFile(target, &FindFileData)) != INVALID_HANDLE_VALUE) {
		do {
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				fileName = dirName + string(FindFileData.cFileName);
				_map_list.push_back(fileName);
			}
		} while (FindNextFile(hnd, &FindFileData));
	}
}

void ResourceManager::loadMap(char * fileName)
{
	for (int i = 0; i < ObjectManager::object_list.size(); ++i) {
		ObjectManager::removeEntity(ObjectManager::object_list.at(i));
	}
	ObjectManager::object_list.clear();
	//將現有地圖移除，再載入新地圖


}

void ResourceManager::saveMap(string fileName)
{
	int tab_num = 0;
	std::ofstream outFile;
	outFile.open(fileName);

	outFile << "<Level name = \"" << fileName << "\" >\n";
	for (int i = 0; i < ObjectManager::object_list.size(); ++i) {
		if (ObjectManager::object_list.at(i)->e_type == BACKGROUND_ENTITY) {
			string texture = ObjectManager::object_list.at(i)->texture;
			float pos_x = ObjectManager::object_list.at(i)->rigid.data.position.x;
			float pos_z = ObjectManager::object_list.at(i)->rigid.data.position.z;
			float scale_x = ObjectManager::object_list.at(i)->_height;
			float scale_z = ObjectManager::object_list.at(i)->_width;
			outFile << "\tbackground name = \"" << texture << " />\n";
			outFile << "\t\t<position x = " << pos_x << " />\n";
			outFile << "\t\t<position z = " << pos_z << " />\n";
			outFile << "\t\t<scale x = " << scale_x << " />\n";
			outFile << "\t\t<scale z = " << scale_z << " />\n";
			outFile << "\t</background>\n";
		}
	}
	for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
		if (ObjectManager::getUI().at(i)->_ui_type == WALL_UI) {
			float point1_x = ObjectManager::getUI().at(i)->_position.x;
			float point1_z = ObjectManager::getUI().at(i)->getZ();
			float point2_x = ObjectManager::getUI().at(i)->_height_width.x + point1_x;
			float point2_z = ObjectManager::getUI().at(i)->_height_width.y + point1_z;
			outFile << "\t<wall>\n";
			outFile << "\t\t<point1 x = " << point1_x << " />\n";
			outFile << "\t\t<point1 z = " << point1_z << " />\n";
			outFile << "\t\t<point2 x = " << point2_x << " />\n";
			outFile << "\t\t<point2 z = " << point2_z << " />\n";
			outFile << "\t</wall>\n";
		}
	}
	outFile << "</Level>";

	outFile.close();
}

vector<string>& ResourceManager::getImageList()
{
	return _image_list;
}

vector<string>& ResourceManager::getMapList()
{
	return _map_list;
}
