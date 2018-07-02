#include "ResourceManager.h"

string ResourceManager::_file_name;
string ResourceManager::_path_name;
vector<string> ResourceManager::_prefab_list;
vector<string> ResourceManager::_image_list;
vector<string> ResourceManager::_map_list;

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}


void ResourceManager::loadPrefabList(char * fileName)
{
	ifstream file;
	file.open(fileName);
	string name;
	while (file >> name) {
		_prefab_list.push_back(name);
	}
}

void ResourceManager::loadResource(char * fileName)
{
	ifstream file;
	file.open(fileName);
	string content;
	string target;
	
	
	while (std::getline(file, content, ' ')) {
		if (content == "\n<Path") {
			target = "Path";
		}
		if (content == "</Path>") {
			_path_name = "";
		}
		if (content == "<Image") {
			target = "Image";
		}
		if (strncmp(content.c_str(), "\"", 1) == 0) {
			string name;
			string key;
			key = content.substr(1, content.size() - 6);
			name = content.substr(1, content.size() - 2);
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
	}


}

/*
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
*/



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

void ResourceManager::loadMap(string fileName)
{
	while (!ObjectManager::object_list.empty()) {
		if (ObjectManager::object_list.back()->e_type != "PLAYER") {
			ObjectManager::removeEntity(ObjectManager::object_list.back());
		}
		else {
			ObjectManager::removePlayer(ObjectManager::object_list.back());
		}
	}
	for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
		if (ObjectManager::getUI().at(i)->_editor) {
			ObjectManager::removeUI(ObjectManager::getUI().at(i));
			--i;
		}
	}
	//將現有地圖移除，再載入新地圖
	PlatformObject *plat = new PlatformObject();
	ObjectManager::addEntity(plat);
	Player *player = new Player(glm::vec3(0, 1, 0));
	ObjectManager::addPlayer(player);
	//載入地面及角色
	ifstream inFile;
	inFile.open(fileName);
	if (!inFile) {
		return;
	}
	string text;
	string object;
	string parameter;
	string cood;
	string texture;
	float x1, x2, z1, z2;
	while (std::getline(inFile, text, ' ')) {

		if (text == "\t<background") {
			object = "background";
		}
		else if (text == "name") {
			parameter = "name";
		}
		else if (strncmp(text.c_str(), "\"", 1) == 0) {
			texture = text.substr(1, text.size() - 2);
		}
		else if (text == "\t\t<position") {
			parameter = "position";
		}
		else if (text == "\t\t<scale") {
			parameter = "scale";
		}
		else if (text == "x") {
			cood = "x";
		}
		else if (text == "z") {
			cood = "z";
		}
		else if (text == "\t</background>\n") {		//載入背景
			object = "";
			parameter = "";
			BackgroundObject *back = new BackgroundObject(texture, glm::vec3(x1, 0, z1), glm::vec3(x2 + x1, 0, z2 + z1));
			ObjectManager::addEntity(back);
		}
		else if (text == "<wall>") {
			object = "wall";
		}
		else if (text == "\t\t<point1") {
			parameter = "point1";
		}
		else if (text == "\t\t<point2") {
			parameter = "point2";
		}
		else if (text == "\t</wall>\n") {			//載入牆壁
			object = "";
			parameter = "";
			WallObject *wall = new WallObject(glm::vec3(x1, 0, z1), glm::vec3(x2, 0, z2));
			ObjectManager::addEntity(wall);
		}
		else if (strncmp(text.c_str(), "(number)", 8) == 0) {
			if (parameter == "position" || parameter == "point1") {
				if (cood == "x") {
					x1 = atof(text.substr(8, text.size() - 8).c_str());
				}
				if (cood == "z") {
					z1 = atof(text.substr(8, text.size() - 8).c_str());
				}
			}
			if (parameter == "scale" || parameter == "point2") {
				if (cood == "x") {
					x2 = atof(text.substr(8, text.size() - 8).c_str());
				}
				if (cood == "z") {
					z2 = atof(text.substr(8, text.size() - 8).c_str());
				}
			}
		}
	}

}

void ResourceManager::loadMap(string fileName, int editor)
{
	while(!ObjectManager::object_list.empty()) {
		if (ObjectManager::object_list.back()->e_type != "PLAYER") {
			ObjectManager::removeEntity(ObjectManager::object_list.back());
		}
		else {
			ObjectManager::removePlayer(ObjectManager::object_list.back());
		}
	}
	for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
		if (ObjectManager::getUI().at(i)->_editor) {
			ObjectManager::removeUI(ObjectManager::getUI().at(i));
			--i;
		}
	}
	//將現有地圖移除，再載入新地圖
	ifstream inFile;
	inFile.open(fileName);
	if (!inFile) {
		return;
	}
	string text;
	string object;
	string parameter;
	string cood;
	string texture;
	float x1, x2, z1, z2;
	while (std::getline(inFile, text, ' ')) {
		
		if (text == "\t<background") {
			object = "background";
		}
		else if (text == "name") {
			parameter = "name";
		}
		else if (strncmp(text.c_str(), "\"", 1) == 0) {
			texture = text.substr(1, text.size() - 2);
		}
		else if (text == "\t\t<position") {
			parameter = "position";
		}
		else if (text == "\t\t<scale") {
			parameter = "scale";
		}
		else if (text == "x") {
			cood = "x";
		}
		else if (text == "z") {
			cood = "z";
		}
		else if (text == "\t</background>\n") {		//載入背景
			object = "";
			parameter = "";
			BackgroundObject *back = new BackgroundObject(texture, glm::vec3(x1, 0, z1), glm::vec3(x2 + x1, 0, z2 + z1));
			ObjectManager::addEntity(back);
		}
		else if (text == "<wall>") {
			object = "wall";
		}
		else if (text == "\t\t<point1") {
			parameter = "point1";
		}
		else if (text == "\t\t<point2") {
			parameter = "point2";
		}
		else if (text == "\t</wall>\n") {			//載入牆壁
			object = "";
			parameter = "";
			WallUI *wall = new WallUI(glm::vec3(x1, 0, z1));
			wall->setPoint2(glm::vec3(x2, 0, z2));
			ObjectManager::addUI(wall);
		}
		else if (strncmp(text.c_str(),"(number)", 8) == 0) {
			if (parameter == "position" || parameter == "point1") {
				if (cood == "x") {
					x1 = atof(text.substr(8, text.size() - 8).c_str());
				}
				if (cood == "z") {
					z1 = atof(text.substr(8, text.size() - 8).c_str());
				}
			}
			if (parameter == "scale" || parameter == "point2") {
				if (cood == "x") {
					x2 = atof(text.substr(8, text.size() - 8).c_str());
				}
				if (cood == "z") {
					z2 = atof(text.substr(8, text.size() - 8).c_str());
				}
			}
		}
	}
}

void ResourceManager::saveMap(string fileName)
{
	int tab_num = 0;
	std::ofstream outFile;
	outFile.open(fileName);

	outFile << "<Level name = \"" << fileName << "\" >\n ";
	for (int i = 0; i < ObjectManager::object_list.size(); ++i) {
		if (ObjectManager::object_list.at(i)->e_type == "BACKGROUND_ENTITY") {
			string texture = ObjectManager::object_list.at(i)->texture;
			float pos_x = ObjectManager::object_list.at(i)->rigid.data.position.x;
			float pos_z = ObjectManager::object_list.at(i)->rigid.data.position.z;
			float scale_x = ObjectManager::object_list.at(i)->_height;
			float scale_z = ObjectManager::object_list.at(i)->_width;
			outFile << "\t<background name = \"" << texture << "\" />\n ";
			outFile << "\t\t<position x = (number)" << pos_x << " />\n ";
			outFile << "\t\t<position z = (number)" << pos_z << " />\n ";
			outFile << "\t\t<scale x = (number)" << scale_x << " />\n ";
			outFile << "\t\t<scale z = (number)" << scale_z << " />\n ";
			outFile << "\t</background>\n ";
		}
	}
	for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
		if (ObjectManager::getUI().at(i)->_ui_type == WALL_UI) {
			float point1_x = ObjectManager::getUI().at(i)->_position.x;
			float point1_z = ObjectManager::getUI().at(i)->getZ();
			float point2_x = ObjectManager::getUI().at(i)->_height_width.x + point1_x;
			float point2_z = ObjectManager::getUI().at(i)->_height_width.y + point1_z;
			outFile << "\t<wall>\n ";
			outFile << "\t\t<point1 x = (number)" << point1_x << " />\n ";
			outFile << "\t\t<point1 z = (number)" << point1_z << " />\n ";
			outFile << "\t\t<point2 x = (number)" << point2_x << " />\n ";
			outFile << "\t\t<point2 z = (number)" << point2_z << " />\n ";
			outFile << "\t</wall>\n ";
		}
	}
	outFile << "</Level> ";

	outFile.close();
}

vector<string>& ResourceManager::getPrefabList()
{
	return _prefab_list;
}

vector<string>& ResourceManager::getImageList()
{
	return _image_list;
}

vector<string>& ResourceManager::getMapList()
{
	return _map_list;
}
