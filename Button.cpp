#include "Button.h"



Button::Button()
{
}

Button::Button(float x, float y, float height, float width) : UI(x, y, height, width)
{
}


Button::~Button()
{
}

void Button::setButtonText(wstring text)
{
	button_text.setText(text);
}

void Button::setButtonText(TString text)
{
	button_text.setText(text);
}

void Button::setButtonText(wstring text, int font_size)
{
	button_text.setText(text);
	button_text.setFontSize(font_size);
}

void Button::setButtonText(TString text, int font_size)
{
	button_text.setText(text);
	button_text.setFontSize(font_size);
}

void Button::setColor(glm::vec4 color)
{
	_color = color;
}

void Button::setColor(float r, float g, float b, float a)
{
	_color = glm::vec4(r, g, b, a);
}

wstring & Button::getText()
{
	return button_text.text.data();
}

TextItem & Button::getTextItem()
{
	return button_text;
}
