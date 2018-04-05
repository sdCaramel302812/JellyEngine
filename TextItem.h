#pragma once
#include "UI.h"
#include "nstdlib.h"
#include <ft2build.h>
#include <map>
//#include "Render.h"
#include FT_FREETYPE_H 


class TextItem : public UI
{
public:

	TextItem();
	TextItem(float x, float y, float width, float height);
	~TextItem();
	void setText(TString txt);
	void setText(wstring txt);
	void setFontSize(int size);
	void setLineWidth(float width);

	virtual void draw();

	TString text;
	float scale = 1;
	int font_size = 48;
	float line_width = 0;

};

