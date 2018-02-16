#pragma once
#include "UI.h"
#include "nstdlib.h"
#include <ft2build.h>
#include <map>
//#include "Render.h"
#include FT_FREETYPE_H 
using nstd::TString;


class TextItem : public UI
{
public:

	TextItem();
	~TextItem();
	void setText(TString txt);
	void setText(wstring txt);
	void setFontSize(int size);
	void setLineWidth(float width);

	virtual void draw();

	TString text;
	float scale;
	int font_size;
	float line_width;

};

