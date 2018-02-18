#include "TextItem.h"
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW ( make you a windows that support opengl operation to work fine with your platform )
#include <GLFW\glfw3.h>



TextItem::TextItem()
{
	font_size = 48;
	scale = 1;
}

TextItem::TextItem(float x, float y, float width, float height)
{
	setXY(x, y);
	setHeight(height);
	setWidth(width);
}


TextItem::~TextItem()
{
}

void TextItem::setText(TString txt)
{
	text = txt;
}

void TextItem::setText(wstring txt)
{
	text = txt;
}

void TextItem::setFontSize(int size)
{
	font_size = size;
	scale = (float)size / 48.0f;
}

void TextItem::setLineWidth(float width)
{
	line_width = width;
}

void TextItem::draw()
{
	Render::drawText(text, x(), y(), scale, glm::vec3(1.0, 1.0, 1.0), line_width, getZ());
}
