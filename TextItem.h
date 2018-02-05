#pragma once
#include "UI.h"
#include "nstdlib.h"
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H 
using nstd::TString;


class TextItem : public UI
{
public:

	TextItem();
	~TextItem();

	TString text;
};

