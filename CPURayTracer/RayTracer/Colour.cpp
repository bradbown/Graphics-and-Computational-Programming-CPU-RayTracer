#include "Colour.h"



Colour::Colour() 
{
	this->red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Colour::Colour(double r, double g, double b, double s) 
{
	red = r;
	green = g;
	blue = b;
	special = s;
}



double Colour::brightness()
{
	return(red + green + blue) / 3;
}

Colour Colour::ColourScalar(double scalar) 
{
	return Colour(red*scalar, green*scalar, blue*scalar, special);
}

Colour Colour::ColourAdd(Colour _Colour)
{
	return Colour(red + _Colour.getColourRed(), green + _Colour.getColourGreen(), blue + _Colour.getColourBlue(), special);
}

Colour Colour::ColourMultiply(Colour _Colour)
{
	return Colour(red*_Colour.getColourRed(), green*_Colour.getColourGreen(), blue*_Colour.getColourBlue(), special);
}

Colour Colour::ColourAverage(Colour _Colour) 
{
	return Colour((red + _Colour.getColourRed()) / 2, (green + _Colour.getColourGreen()) / 2, (blue + _Colour.getColourBlue()) / 2, special);
}

Colour Colour::clip() 
{
	double alllight = red + green + blue;
	double excesslight = alllight - 3;
	if (excesslight > 0) 
	{
		red = red + excesslight*(red / alllight);
		green = green + excesslight*(green / alllight);
		blue = blue + excesslight*(blue / alllight);
	}
	if (red > 1) { red = 1; }
	if (green > 1) { green = 1; }
	if (blue > 1) { blue = 1; }
	if (red < 0) { red = 0; }
	if (green < 0) { green = 0; }
	if (blue < 0) { blue = 0; }

	return Colour(red, green, blue, special);
}
