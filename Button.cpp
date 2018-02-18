#include "Button.h"



Button::Button()
{
}

Button::Button(float x, float y, float width, float height) : UI(x, y, width, height)
{
	_has_hover = true;
	button_text = new TextItem();
	button_text->setXY(x + 20, y + (height - 50 > 0 ? (height - 50) / 2 : 0));
	button_text->setLineWidth(width);
	button_text->setZ(getZ() - 0.1);
	setColor(glm::vec4(0.5, 0.5, 0.5, 0.5));
}


Button::~Button()
{
	delete button_text;
}

void Button::setButtonText(wstring text)
{
	button_text->setText(text);
}

void Button::setButtonText(TString text)
{
	button_text->setText(text);
}

void Button::setButtonText(wstring text, int font_size)
{
	button_text->setText(text);
	button_text->setFontSize(font_size);
}

void Button::setButtonText(TString text, int font_size)
{
	button_text->setText(text);
	button_text->setFontSize(font_size);
}


void Button::draw()
{
	button_text->draw();
	Render::draw(this);
	
}

void Button::hoverIn()
{
	setColor(glm::vec4(0.5, 0.5, 0.5, 0.8));
}

void Button::hoverOut()
{
	setColor(glm::vec4(0.5, 0.5, 0.5, 0.5));
}

wstring & Button::getText()
{
	return button_text->text.data();
}

TextItem & Button::getTextItem()
{
	return *button_text;
}
