#include "TextItem.h"
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW ( make you a windows that support opengl operation to work fine with your platform )
#include <GLFW\glfw3.h>



TextItem::TextItem()
{
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
