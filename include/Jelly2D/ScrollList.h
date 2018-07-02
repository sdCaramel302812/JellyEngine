#pragma once
#include "UI.h"
#include "Button.h"
#include <vector>
#include "ObjectManager.h"

class ScrollList :public UI		//應急用低泛用性
{
public:
	typedef std::function<void(void)> CALL_BACK;

	ScrollList();
	ScrollList(float x, float y, float width, float height);
	void setItemHeight(float height);
	void addItem(string button_name);
	void addItem(string button_name, CALL_BACK function);
	void setFontSize(int size);
	void setVisable(bool value);
	vector<Button *> &getItemList();
	~ScrollList();

	virtual void wheelEvent(float yoffset);

	vector<Button *> _item_list;
	float _scroll_speed;
	float _item_height;
	bool _scroll_up;			//向列表上方滾動，物件向下移動
	bool _scroll_down;			//向列表下方滾動，物件向上移動
	int _font_size = 24;
};

