#pragma once
#include "TextItem.h"
#include "UI.h"
#include "Render.h"
#include "EventManager.h"

/////////////////////////////////	callback 問題	/////////////////////////////////
//目前沒有 first click 機制
//只要滑鼠按著經過就會觸發 callback
/////////////////////////////////	callback 問題	/////////////////////////////////


class Button : public UI
{
public:
	Button();
	Button(float x, float y, float width, float height);
	~Button();

	void setButtonText(wstring text);
	void setButtonText(TString text);
	void setButtonText(wstring text, int font_size);
	void setButtonText(TString text, int font_size);
	
	virtual void draw();

	virtual void hoverIn();
	virtual void hoverOut();

	wstring &getText();
	TextItem &getTextItem();

	TextItem  *button_text;
	glm::vec4 _color;
};

