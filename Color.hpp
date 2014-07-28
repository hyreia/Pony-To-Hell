struct Color
{
public:
	unsigned char r, g, b, a;

	Color();
	Color(ALLEGRO_COLOR color);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);

	operator ALLEGRO_COLOR(){ return al_map_rgba(r, g, b, a); }
	
	Color operator *(const Color &other) const
	{ return al_map_rgba(r*other.r/255, g*other.g/255, b*other.b/255, a*other.a/255); 
	}

	bool operator ==(const Color &other) const
	{
		return (r==other.r && g==other.g && b==other.b && a==other.a);
	}

	bool operator !=(const Color &other) const
	{
		return !((*this) == other);
	}

	/* coefficient 1.0f = destination, 0.5 halfway to destination, 0.75f 3/4 of the way to dest color*/
	Color Lerp(const Color &destination, const float &coefficient) const;

	/* -1.0f for completely black, 1.0f for completely white, transparency unaffected */
	Color SetBrightness(const float brightness) const;

	static const Color Red;
	static const Color Pink;
	static const Color Orange;
	static const Color Yellow;
	static const Color Green;
	static const Color DarkGreen;
	static const Color Blue;
	static const Color BrightBlue;
	static const Color Purple;
	static const Color Black;
	static const Color DarkGrey;
	static const Color Grey;
	static const Color White;

};

struct ColorPair
{
	ColorPair():
	fore(Color::White), back(Color::Black)
	{}
	ColorPair(const Color fore, const Color back):
	fore(fore), back(back)
	{}

	void Set(const Color fore, const Color back)
	{
		this->fore = fore; this->back = back;
	}

	Color fore, back;
};

enum ControlColors
{
	COLOR_PAIR_1 = 1,
	COLOR_PAIR_2,
	COLOR_PAIR_3,
	COLOR_PAIR_4,
	TOTAL_COLOR_PAIRS=COLOR_PAIR_4,
	DEFAULT_CONSOLE_COLORS
};