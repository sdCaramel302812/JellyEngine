#pragma once
#include "TextItem.h"
#include "UI.h"
#include "Render.h"
#include "EventManager.h"



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

	virtual void setXY(float x, float y);

	wstring &getText();
	TextItem &getTextItem();

	TextItem  *button_text;
	glm::vec4 _color;
};

