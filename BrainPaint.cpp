#include <iostream>
#include <vector>
#include <string>
#include <fstream>


// CREDITS: http://www.cplusplus.com/forum/general/6194/
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

// end Credits

//next byte
void next();
//last byte
void last();
//add one to current byte
void add();
//subtract one from current byte
void substract();
//start a loop
void beginLoop();
//end of a loop, repeat if not ending yet
void endLoop();
//draw a pixel using the next 5 bytes [x,y,r,g,b]
void drawPixel();
//print current byte char value
void printChar();

//the current position of the byte we want to use
int curByte;
//the bytes we can use
int bytes[1024];
//the size and datot of the image we will generate
RGB_t data[100][100];
//all the starts of the loops
std::vector<int> loopBegins;
//a string which will be filled with all the commands
std::string commands;
//the current position of the command we are executing in the string
int commandIndex;

int main()
{
	//variable inits
	commandIndex = 0;
	commands = "";
	char c = 0;
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

	//open the file
	std::ifstream file("brainfuck.txt");
	//return if there is no file present
	if (!file.is_open())
		return -1;
	//add all commands to the commans strings
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
	//close the file
	file.close();
	//start executing the commands
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
	//output the image
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
	//if the current byte is 0 we can leave the loop
	if (bytes[curByte] == 0)
	{
		loopBegins.pop_back();
	}
	else
	{
		//if the loop isnt done, repeat
		commandIndex = (loopBegins.back());
	}
}

void drawPixel()
{
	//if the variables get out of range dont try to use them
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
}

void printChar()
{
	std::cout << (char)bytes[curByte];
}
