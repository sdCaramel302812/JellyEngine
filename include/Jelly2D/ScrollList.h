#pragma once
#include "UI.h"
#include "Button.h"
#include <vector>
#include "ObjectManager.h"

class ScrollList :public UI		//����ΧC�x�Ω�
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
	bool _scroll_up;			//�V�C��W��u�ʡA����V�U����
	bool _scroll_down;			//�V�C��U��u�ʡA����V�W����
	int _font_size = 24;
};

