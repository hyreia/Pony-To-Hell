#include "main.hpp"

const Color Color::Red(255, 0, 0);
const Color Color::Pink(255, 128, 128);
const Color Color::Orange(255, 128, 0);
const Color Color::Yellow(255, 255, 0);
const Color Color::Green(128, 255, 128);
const Color Color::DarkGreen(64,128,64);
const Color Color::Blue(128, 128, 255);
const Color Color::BrightBlue(0, 0, 255);
const Color Color::Purple(128, 0, 255);
const Color Color::Black(0,0,0);
const Color Color::Grey(128, 128, 128);;
const Color Color::White(255,255,255);
const Color Color::DarkGrey(64, 64, 64);

Color::Color(): r(0), g(0), b(0), a(0)
{}

Color::Color(ALLEGRO_COLOR color)
{
	al_unmap_rgba(color, &r, &g, &b, &a);
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a):
r(r), g(g), b(b), a(a)
{}

Color Color::Lerp(const Color &dest, const float &coef) const
{
	Color ret;
	ret.r=(unsigned char)(r+(dest.r-r)*coef);
	ret.g=(unsigned char)(g+(dest.g-g)*coef);
	ret.b=(unsigned char)(b+(dest.b-b)*coef);
	return ret;
}

Color Color::SetBrightness(const float brightness) const
{
	return brightness > 0?	Lerp( Color(255,255,255, a), brightness):
							Lerp( Color(0, 0, 0, a), 0-brightness);
}
