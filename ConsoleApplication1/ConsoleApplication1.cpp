// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

void next();
void last();
void add();
void substract();
void beginLoop();
void endLoop();
void gotoByte();
void drawPixel();
void printChar();

typedef unsigned char byte;
typedef struct
{
	byte red, green, blue;
}
RGB_t;

// It is presumed that the image is stored in memory as 
//   RGB_t data[ height ][ width ]
// where lines are top to bottom and columns are left to right
// (the same way you view the image on the display)

// The routine makes all the appropriate adjustments to match the TGA format specification.

bool write_truecolor_tga(const std::string& filename, RGB_t* data, unsigned width, unsigned height)
{
	std::ofstream tgafile(filename.c_str(), std::ios::binary);
	if (!tgafile) return false;

	// The image header
	byte header[18] = { 0 };
	header[2] = 1;  // truecolor
	header[12] = width & 0xFF;
	header[13] = (width >> 8) & 0xFF;
	header[14] = height & 0xFF;
	header[15] = (height >> 8) & 0xFF;
	header[16] = 24;  // bits per pixel

	tgafile.write((const char*)header, 18);

	// The image data is stored bottom-to-top, left-to-right
	for (int y = height - 1; y >= 0; y--)
		for (int x = 0; x < width; x++)
		{
			tgafile.put((char)data[(y * width) + x].blue);
			tgafile.put((char)data[(y * width) + x].green);
			tgafile.put((char)data[(y * width) + x].red);
		}

	// The file footer. This part is totally optional.
	static const char footer[26] =
		"\0\0\0\0"  // no extension area
		"\0\0\0\0"  // no developer directory
		"TRUEVISION-XFILE"  // yep, this is a TGA file
		".";
	tgafile.write(footer, 26);

	tgafile.close();
	return true;
}


int curByte;
int bytes[1024];
RGB_t data[100][100];
std::vector<int> loopBegins;
std::string commands;
//std::vector<void(*)()> functions;
int commandIndex;
int main()
{
	commandIndex = 0;
	commands = "";
	std::ifstream file("brainfuck.txt");
	char c = 0;
	while (file.get(c))
	{
		switch (c)
		{
		case '+':
			commands += c;
			break;
		case '-':
			commands += c;
			break;
		case '>':
			commands += c;
			break;
		case '<':
			commands += c;
			break;
		case '[':
			commands += c;
			break;
		case ']':
			commands += c;
			break;
		case '.':
			commands += c;
			break;
		case '$':
			commands += c;
			break;
		}

	}
	file.close();
	for (int i = 0; i < 1024; i++)
	{
		bytes[i] = 0;
	}

	for (unsigned int i = 0; i < 100; i++)
	{
		for (unsigned int j = 0; j < 100; j++)
		{
			data[i][j].red = 0;
			data[i][j].green = 0;
			data[i][j].blue = 0;
		}
	}

	for (commandIndex = 0; commandIndex < commands.length(); commandIndex++)
	{
		switch (commands.at(commandIndex))
		{
		case '+':
			add();
			break;
		case '-':
			substract();
			break;
		case '>':
			next();
			break;
		case '<':
			last();
			break;
		case '[':
			beginLoop();
			break;
		case ']':
			endLoop();
			break;
		case '.':
			printChar();
			break;
		case '$':
			drawPixel();
			break;
		}
	}
	write_truecolor_tga("output.tga", (*data), 100, 100);
	return 0;
}

void next()
{
	if (curByte < 1024)
	{
		curByte++;
	}
}

void last()
{
	if (curByte >= 0)
	{
		curByte--;
	}
}

void add()
{
	bytes[curByte]++;
}

void substract()
{
	bytes[curByte]--;
}

void beginLoop()
{
	loopBegins.push_back(commandIndex);
}

void endLoop()
{
	if (bytes[curByte] == 0)
	{
		loopBegins.pop_back();
	}
	else
	{
		commandIndex = (loopBegins.back());
	}
}

void gotoByte()
{
	curByte = bytes[curByte];
}

void drawPixel()
{
	std::cout << "DawPixel" << std::endl;
	if (curByte + 4 >= 1024)
		return;
	int x = bytes[curByte];
	int y = bytes[curByte + 1];
	int r = bytes[curByte + 2];
	int g = bytes[curByte + 3];
	int b = bytes[curByte + 4];
	data[x][y].red = r;
	data[x][y].blue = b;
	data[x][y].green = g;
	std::cout << x << ":" << y << ":" << r << ":" << g << ":" << b << std::endl;
}

void printChar()
{
	std::cout << (char)bytes[curByte];
}
