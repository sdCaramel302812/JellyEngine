#pragma once
#include "TextItem.h"
#include "UI.h"

class Button : public UI
{
public:
	Button();
	Button(float x, float y, float height, float width);
	~Button();

	void setButtonText(wstring text);
	void setButtonText(TString text);
	void setButtonText(wstring text, int font_size);
	void setButtonText(TString text, int font_size);
	
	void setColor(glm::vec4 color);
	void setColor(float r, float g, float b, float a = 1);

	wstring &getText();
	TextItem &getTextItem();

	TextItem button_text;
	glm::vec4 _color;
};

