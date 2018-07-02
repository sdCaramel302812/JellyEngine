#include "Button.h"



Button::Button()
{
}

Button::Button(float x, float y, float width, float height) : UI(x, y, width, height)
{
	_has_hover = true;
	button_text = new TextItem();
	button_text->setXY(x + button_text->font_size / 2, y + (this->height() - button_text->font_size * 3 / 2));
	button_text->setLineWidth(width);
	setZ(1);
	button_text->setZ(getZ() + 1);
	setColor(glm::vec4(0.5, 0.5, 0.5, 1.0));
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
	button_text->setXY(_position.x + button_text->font_size / 2, _position.y + (this->height() - button_text->font_size * 3 / 2));
}

void Button::setButtonText(TString text, int font_size)
{
	button_text->setText(text);
	button_text->setFontSize(font_size);
	button_text->setXY(_position.x + button_text->font_size / 2, _position.y + (this->height() - button_text->font_size * 3 / 2));
}


void Button::draw()
{
	button_text->draw();
	Render::draw(this);
	
}

void Button::hoverIn()
{
	if (this->_texture == "") {
		setColor(glm::vec4(0.4, 0.4, 0.4, 1.0));
	}
	else if (this->_texture2 != "") {
		setTexture(this->_texture2);
	}
}

void Button::hoverOut()
{
	if (this->_texture == "") {
		setColor(glm::vec4(0.5, 0.5, 0.5, 1.0));
	}
	else {
		setTexture(this->_texture);
	}
}

void Button::setXY(float x, float y)
{
	_position.x = x;
	_position.y = y;
	button_text->setXY(x + button_text->font_size / 2, y + (this->height() - button_text->font_size * 3 / 2));
}

wstring & Button::getText()
{
	return button_text->text.data();
}

TextItem & Button::getTextItem()
{
	return *button_text;
}
