#include "main.hpp"

using namespace DBL;

TextType::TextType(const char *filename, int size):
font(NULL)
{
	font = al_load_ttf_font(filename, size, 0);
}

TextType::~TextType()
{
	al_destroy_font(font);
}