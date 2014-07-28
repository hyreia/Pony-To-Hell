#ifndef TEXT_TYPE_HPP
#define TEXT_TYPE_HPP

namespace DBL
{
	class TextType
	{
	public:
		TextType(const char *filename, int size);
		~TextType();
		ALLEGRO_FONT *font;
	};
}


#endif