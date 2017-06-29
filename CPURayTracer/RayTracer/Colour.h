#pragma once

class Colour 
{
public:

	Colour();

	Colour(double, double, double, double);

	double red, green, blue, special;

	// method functions
	double getColourRed() { return red; }
	double getColourGreen() { return green; }
	double getColourBlue() { return blue; }
	double getColourSpecial() { return special; }

	double setColourRed(double redValue) { red = redValue; }
	double setColourGreen(double greenValue) { green = greenValue; }
	double setColourBlue(double blueValue) { blue = blueValue; }
	double setColourSpecial(double specialValue) { special = specialValue; }

	double brightness();

	Colour ColourScalar(double scalar);

	Colour ColourAdd(Colour Colour);

	Colour ColourMultiply(Colour Colour);

	Colour ColourAverage(Colour Colour);

	Colour clip();
};