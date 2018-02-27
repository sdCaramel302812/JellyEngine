#include "ScrollList.h"



ScrollList::ScrollList()
{
	_scroll_up = true;
	_scroll_down = true;
	_scroll_speed = 100;
	_has_wheel_event = true;
	setItemHeight(80.0f);
}

ScrollList::ScrollList(float x, float y, float width, float height) :UI(x, y, width, height)
{
	_scroll_up = true;
	_scroll_down = true;
	_scroll_speed = 100;
	_has_wheel_event = true;
	_visable = false;
	setItemHeight(80.0f);
}

void ScrollList::setItemHeight(float height)
{
	_item_height = height;
}

void ScrollList::addItem(string button_name)
{
	Button *button = new Button(this->x(), this->y() + this->height() - _item_height*(_item_list.size() + 1), this->width(), _item_height);
	button->setButtonText(button_name);
	_item_list.push_back(button);
	if (button->y() < this->y()) {
		button->setVisable(false);
		button->button_text->setVisable(false);
	}
	ObjectManager::addUI(button);
	ObjectManager::addText(button->button_text);
}

void ScrollList::addItem(string button_name, CALL_BACK function)
{
	Button *button = new Button(this->x(), this->y() + this->height() - _item_height*(_item_list.size() + 1), this->width(), _item_height);
	button->setButtonText(button_name);
	button->setCallback(function);
	_item_list.push_back(button);
	if (button->y() < this->y()) {
		button->setVisable(false);
		button->button_text->setVisable(false);
	}
	ObjectManager::addUI(button);
	ObjectManager::addText(button->button_text);
}

void ScrollList::setVisable(bool value)
{
	this->setEnable(value);
	for (int i = 0; i < _item_list.size(); ++i) {
		_item_list.at(i)->setVisable(value);
		_item_list.at(i)->button_text->setVisable(value);
		if (_item_list.at(i)->y() < this->y() || _item_list.at(i)->y() > this->y() + this->height()) {
			_item_list.at(i)->setVisable(false);
			_item_list.at(i)->button_text->setVisable(false);
		}
	}
}

vector<Button*>& ScrollList::getItemList()
{
	return _item_list;
}


ScrollList::~ScrollList()
{
	for (int i = 0; i < _item_list.size(); ++i) {
		ObjectManager::removeUI(_item_list.at(i));
		ObjectManager::removeText(_item_list.at(i)->button_text);
	}
	_item_list.clear();
}

void ScrollList::wheelEvent(float yoffset)
{
	if (_item_list.empty()) {
		return;
	}
	if (_item_list.at(0)->y() < this->y() + this->height() - _item_height){
		_scroll_up = false;
	}
	else {
		_scroll_up = true;
	}
	if (_item_list.back()->y() > this->y()) {
		_scroll_down = false;
	}
	else {
		_scroll_down = true;
	}
	float dy = -_scroll_speed * yoffset;
	for (int i = 0; i < _item_list.size(); ++i) {
		if ((dy < 0 && _scroll_up) || (dy > 0 && _scroll_down)) {
			_item_list.at(i)->setXY(_item_list.at(i)->x(), _item_list.at(i)->y() + dy);
		}
		if (_item_list.at(i)->y() < this->y() || _item_list.at(i)->y() > this->y() + this->height()) {
			_item_list.at(i)->setVisable(false);
			_item_list.at(i)->button_text->setVisable(false);
		}
		else {
			_item_list.at(i)->setVisable(true);
			_item_list.at(i)->button_text->setVisable(true);
		}
	}
}
