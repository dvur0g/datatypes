#include <iostream>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <limits.h>
#include <string>
#include <float.h>
#include <algorithm>
#pragma comment(lib, "winmm.lib")

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int PRIMARY_COLOR = 0xf0, SECOND_COLOR = 0xc9;

const int UP_KEY = 119, DOWN_KEY = 115, ENTER_KEY = 13, ESC_KEY = 27,
			UP_ARROW_KEY = 72, DOWN_ARROW_KEY = 80, LEFT_ARROW_KEY = 75, RIGHT_ARROW_KEY = 77;

const int BIAS = 49, dot = 46, ANSWER_YES = 121, ANSWER_NO = 110;

const char INPUT_VALUES[36] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
const int MAX_INPUT_ARRAY = 100;

const char* EMPTY_LINE = "                                                                                                                        ";
const int coordInversion = 4, yInversion = 13;

const char BLOCK = 219;

void gotoxy(int x, int y) {
	COORD coord;	
  	coord.X = x;
  	coord.Y = y;
  	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int input(char* str, int min, int max) {
	int result = min - 1;
	while(result <= min || result >= max) {
		std::cout << str;
		std::cin >> result;
	}	
	return result;
}

int menu(char* text) {
	int i = 0, count = 0;
	while(text[i]) {
		if(text[i] == '\n')
			count++;
		i++;
	}
	int type = 0;
	int y = 0;
	const int xArrow = 22;
	const std::string arrow = "<---";
	const std::string empty_arrow = "    ";

	system("cls");
	
	std::cout << text;
	std::cout << "\n Esc - Quit";
	
	gotoxy(xArrow, type);
	std::cout << arrow;
	while(true) {
		int a = getch();
		if(a > BIAS - 1 && a < BIAS + count) {
			type = a - BIAS;
			break;
		} else if((a == DOWN_ARROW_KEY || a == DOWN_KEY) && type < count - 1) {
			gotoxy(xArrow, type);
			std::cout<< empty_arrow;
			gotoxy(xArrow, ++type);
			std::cout<< arrow;
		} else if((a == UP_ARROW_KEY || a == UP_KEY) && type > 0) {
			gotoxy(xArrow, type);
			std::cout<< empty_arrow;
			gotoxy(xArrow, --type);
			std::cout<< arrow;
		} else if(a == ENTER_KEY)
			break;
		else if(a == ESC_KEY)
			return count;
	}
	
	system("cls");
	return type;
}

void printArray(const char *a, bool reverse = false, int scale = 0){
	int i = 0;
	if(scale != 0) {
		while(i < scale) {
			std::cout << a[i] << ", "; ++i;
		} std::cout << std::endl;
		return;
	}
	if(!reverse) {
        std::cout << "\n1: ";
    	while(a[i]) {
    	    std::cout << a[i] << ", "; ++i;
    	}
    } else {
    	while(a[i]) {
    	    ++i;
    	} i--;
    	
    	std::cout << "\nBinary form: ";
    	while(i >= 0) {
    	    std::cout << a[i];
			
    		if((i % 8) == 0)
    			std::cout << ' ';
			
			--i;
    	} 
	} std::cout << std::endl;
}

void printInvertArray(char *a, int from, int to){
    int i = 0;
	while(a[i]) {
   	    ++i;
   	} --i;
    	
    std::cout << "\nBinary form: ";
    while(i >= 0) {
    	if(i <= from && i >= to)
        	SetConsoleTextAttribute(hConsole, SECOND_COLOR);
		std::cout << a[i];

		SetConsoleTextAttribute(hConsole, PRIMARY_COLOR);
    	if((i % 8) == 0)
    		std::cout << ' ';
			
		--i;     
	} std::cout << std::endl;
}

int checkForDot(char* a, int length) {
	int i = 0;
	while(i < length) {
		if(a[i] == dot)
		    return i;
		++i;
	}
	return -1;
}

template <typename T>
T toDecScale(char* a, int scale, T r, int length) {
	int dot = checkForDot(a, length);
	int i = dot + 1, step = -1;
	long long value;
	T result = 0;
	
	if(dot != -1) {
		while(i < length) {
			for(int j = 0; j < 36; ++j) {
	    		if(a[i] == INPUT_VALUES[j]) {
	    		    value = j;
	    		    break;  
				}
			}
			result += value * pow(scale, step);
	    	--step; ++i;
		}
		i = dot - 1;
	} else {
		i = length;
	}
		
	step = 0; 
    while(i >= 0 && a[i] != '-') {
    	for(int j = 0; j < 36; ++j) {
    		if(a[i] == INPUT_VALUES[j]) {
    		    value = j;
    		    break;    
			}
		}
    		result += value * pow(scale, step);
    	++step; --i;
	}
	
	if(a[0] == '-')
	{
		double temp = pow(2, sizeof(T)*8 - 1);
		//result = -result;
		__asm
		{
		 	MOV zmm0, (temp);
		 	XOR result, mm0;
		}
	}
	return result;   
}

long long toDecScale(char* a, int length) {
	long long result = 0;
	
	int i = length - 1;
	
	int step = 0, value = 0; 
    while(i >= 0) {
    	for(int j = 0; j < 36; ++j) {
    		if(a[i] == INPUT_VALUES[j]) {
    		    value = j;
    		    break;    
			}
		}
    		result += value * pow(2, step);
    	++step; --i;
	}
    return result;   
}

bool checkForBoundaries(char* a, int scale, long long min, long long max, int length) {
	if(min == 4 || min == 5)
		return false;
	
	long long num = toDecScale(a, scale, num, length);
	
	if((long long)num < min || (long long)num > max)
		return true;
	return false;
}

int getLength(char* a)
{
	int i = 0;
	while(a[i] != '\0')
		++i;
	return --i;
}

char* input(int scale, int type) {
	long long min = (long long)type, max;
	if(type == 0) {
		min = CHAR_MIN;
		max = CHAR_MAX;
	} else if(type == 1) {
		min = INT_MIN;
		max = INT_MAX;
	} else if(type == 2) {
		min = LONG_MIN;
		max = LONG_MAX;
	} else if(type == 3) {
		min = -127;
		max = 128;
	} else if(type == 4 || type == 5) {
		min = type;
		max = 0;
	}
	
	std::cout << ", scale of notation: " << scale;
	int x = 0, y = 1;
	gotoxy(x, y++);
	std::cout << "Characters: ";
	printArray(&*INPUT_VALUES, false, scale);
	
	
	char* destination = new char[MAX_INPUT_ARRAY];
	int length = 0;
	memset(destination, '\0', MAX_INPUT_ARRAY);
	
	char k = 0, i = 0;
	while(i < MAX_INPUT_ARRAY) {
		k = getch(); //48-57
        
        if(k == 13)
        	break;
        gotoxy(60, 6);
	    std::cout << "                                     ";
	    gotoxy(x, y);
			
        if(k == 8) { //backspace
        	if(i > 0) {
        	    destination[--i] = '\0';
        	    gotoxy(--x, y);
        	    std::cout << " ";
        	    gotoxy(x, y);
            }
			continue;
		}
		else if(k == 45) { //minus
			if(i != 0)
				continue;
		}
		else if(k == dot && i != 0) { //dot
			if(checkForDot(destination, getLength(destination)) != -1)
				continue;
		}
        else if(scale <= 10) {
		    if(k < 48 || k > 48 + scale - 1)
                continue;
        } else {
        	if(k < 48 || k > 57 && k != 46)
     		    if(k < 97 || k > 97 + scale - 10 - 1)
	    	       continue;
        }
        
		std::cout << k;
		x++;
	    destination[i] = k;

	    if(checkForBoundaries(destination, scale, min, max, getLength(destination))) {
	    	gotoxy(60, 6);
	    	std::cout << "Out of bound!";
	    	gotoxy(x, y);
			
			destination[i] = '\0';
			gotoxy(--x, y);
        	std::cout << " ";
        	gotoxy(x, y);
		} else
			++i;
	}
	return destination;
}

static union {
	double d_union;
	long long l_union;

	float f_union;
	int i_union;
};

template <typename T>
char* toBinScale(T num) {
	char* result = new char[MAX_INPUT_ARRAY];
	memset(result, '\0', MAX_INPUT_ARRAY);
	
	int i = 0;
	while(i < sizeof(T)*8) {
		if(num & 1)
			result[i] = '1'; 
		else
		    result[i] = '0';
		num >>= 1;
		++i;
	}
	return result;
}

template <typename T>
void printResult(T result) {
	std::cout << "\nC++ representation: " << (T)result;
}

template<typename T>
T invertBits(T &num)
{
	int from; int to;
	from = input("\nIndex of the first bit to invert: ", -1, sizeof(num)*8);
	to = input("\nIndex of the second bit: ", -1, sizeof(num)*8);
	
	if (from < to)
		std::swap(from, to);
	
	for (int i = to; i <= from; i++)  
       num = (num ^ ((T)1 << i));  

	printInvertArray(toBinScale(num), from, to);	
	return num;
}

template<typename T>
void modulation(char* in, T type)
{
	//for graph 
	int x = 2, y = 20;
	int yStart = y*(1.7); 

	int size = sizeof(T)*8;
	int batch = input("\nNumber of bites to module: ", 0, size);
	
	int impulseAmount = size/batch;
	if(size % batch != 0)
		impulseAmount++;
	
	long long *modulArray = new long long[impulseAmount];
	long long *modulArrayGraph = new long long[impulseAmount];
	int j = 0;
	char* temp = new char[batch];

	gotoxy(x, yStart + 1);
	for(int i = 0; i < impulseAmount; ++i)
	{
		for(int k = 0; k < batch; ++k)
		{
			if(k + i*batch < size)
				temp[k] = in[k + i*batch];
			else 
				temp[k] = '0';
		}
		std::reverse(temp, temp + batch);

		if(i % 2 != 0)
			SetConsoleTextAttribute(hConsole, SECOND_COLOR);
		for(int l = 0; l < batch; l++)
		    std::cout << temp[l];
		SetConsoleTextAttribute(hConsole, PRIMARY_COLOR);
		
		modulArray[j] = toDecScale(temp, batch) + 1;
		modulArrayGraph[j] = modulArray[j];
		++j;
	}
	
	if(batch > 4)
		for(int i = 0; i < impulseAmount; i++)
		{
			modulArrayGraph[i] >>= batch - 4;
			if(modulArrayGraph[i] == 0)
				modulArrayGraph[i]++;
		}
	
	for(int i = 0; i < impulseAmount; i++)
	{
		for(int j = yStart; j > yStart - modulArrayGraph[i]; --j)
		{
			gotoxy(x, j);
			if(i % 2 != 0)
				SetConsoleTextAttribute(hConsole, SECOND_COLOR);

			for(int k = 0; k < batch; k++)
				std::cout << BLOCK;
			SetConsoleTextAttribute(hConsole, PRIMARY_COLOR);
		}
		gotoxy(x += batch, y);
	}
	gotoxy(0, yStart + 2);
}

void clearLines(int y, int len)
{
	int max = len + y;
	for(int i = y; i < max; i++)
	{
		gotoxy(0, i);
		std::cout << EMPTY_LINE;
	}
}

int moveCursor(int n, int m, int scale) {
	int dir, x = 0, xc = 0, y = 0, yc = 0;
	gotoxy(x, y);
	
	while(true) {
		dir = getch();
		if(dir == UP_ARROW_KEY && yc > 0) {
			y -= 2;
			--yc;
			gotoxy(x, y);
		} else if(dir == DOWN_ARROW_KEY && yc < n) {
			y += 2;
			++yc;
			gotoxy(x, y);
		} else if(dir == LEFT_ARROW_KEY && xc > 0) {
			--xc;
			x -= scale;
			gotoxy(x, y);
		} else if(dir == RIGHT_ARROW_KEY && xc < m) {
			++xc;
			x += scale;
			gotoxy(x, y);
		} else if(dir == ENTER_KEY)
			return xc + yc*(m + 1);
		  else if(dir == ESC_KEY) 
			return -1;
		gotoxy(x, y);
	}
	return -1;
}

void clearScreen(int X_MAX, int Y_MAX)
{
  std::string str;
  gotoxy(0, 0);
  for(int i = 0; i < Y_MAX; i++)
  {
    for(int j = 0; j <= X_MAX; j++)
      str.append(" ");

    str.push_back('\n');
  }
  std::cout << str;
}

void drawSound()
{
	const int sampleRate = 8000;
	const int latency = 1;
	const int millisec = 500;

	const int NUMPTS = sampleRate * latency;
	short int waveIn[sampleRate * latency];
	
	WAVEFORMATEX pFormat;
	{
		// Specify recording parameters
		pFormat.wFormatTag = WAVE_FORMAT_PCM; // simple, uncompressed format
		pFormat.nChannels = 1;
		pFormat.nSamplesPerSec = sampleRate; // 44100
		pFormat.nAvgBytesPerSec = sampleRate * 2; // = nSamplesPerSec * n.Channels * wBitsPerSample/8 //16 bites
		pFormat.nBlockAlign = 2; // = n.Channels * wBitsPerSample/8
		pFormat.wBitsPerSample = 16;
		pFormat.cbSize = 0;
	}

	WAVEHDR WaveInHdr;
	{
		// Set up and prepare header for input
		WaveInHdr.lpData = (LPSTR)waveIn;
		WaveInHdr.dwBufferLength = NUMPTS*2;
		WaveInHdr.dwBytesRecorded=0;
		WaveInHdr.dwUser = 0L;
		WaveInHdr.dwFlags = 0L;
		WaveInHdr.dwLoops = 0L;
	}
	HWAVEIN hWaveIn;

	long long temp = 0;
	int x = 0, x_max = 2, X_MAX = 115, Y_MAX = 30, y_max = 10, bias = 0;
	bool ok = true;
	
	while(!kbhit())
	{
		while(x < X_MAX)
		{
			waveInOpen(&hWaveIn, WAVE_MAPPER,&pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);
			waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
			waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

			waveInStart(hWaveIn);
			Sleep(latency * millisec);
			waveInStop(hWaveIn);
			waveInReset(hWaveIn);
			
			for(int i = 0; i < sampleRate; i++)
				temp += std::abs(waveIn[i]);
			
			
			y_max = temp / sampleRate / 90 - 12; //rand()%19 + 1; //get sound here (average) /3276.8/60/80
			gotoxy(0, 0);
			std::cout << y_max;
			if(y_max <= 0)
				y_max = 1;
			else if(y_max > Y_MAX)
				y_max = Y_MAX;
			if(y_max < Y_MAX)
			{
				bias = (Y_MAX - y_max)/2;
			}
			else
				bias = 0;
			
			for(int i = x; i < x + x_max; ++i)
			{
				for(int j = bias; j < bias + y_max; ++j)
				{
					if(j >= bias)
					{
						gotoxy(i, j);
						std::cout << BLOCK;
					}
					if(kbhit())
						return;

				}
			}
			x += x_max;
			temp = 0;
		}
		x = 0;
		clearScreen(X_MAX, Y_MAX + 5);
	}
}

template<typename T>
void typenameInput(T &value, int type)
{
	char* array = new char[MAX_INPUT_ARRAY]; 
	
	int scale = input("Input notation (form 2 to 36): ", 1, 37);
	system("cls");

	std::cout << "Data type: " << typeid(value).name();
	array = input(scale, type);
	int length = getLength(array);

	value = toDecScale(array, scale, value, length); 
	printResult(value);
	
	int repeat = ANSWER_YES;
	while(repeat == ANSWER_YES)
	{
		clearLines(coordInversion, yInversion);
						
		gotoxy(0, coordInversion);
		printArray(toBinScale(value), true);
		invertBits(value);

		std::cout << "\nResult: " << value 
					<< "\nDo invertion again? (y/n) ";
		repeat = getch();
	}
	modulation(toBinScale(value), value);
}

template<typename T>
void doubleFloatInput(T &value, int type)
{
	char* array = new char[MAX_INPUT_ARRAY]; 
	
	int scale = input("Input notation (from 2 to 36): ", 1, 37);
	system("cls");

	std::cout << "Data type: " << typeid(value).name();
	array = input(scale, type);
	int length = getLength(array);

	value = toDecScale(array, scale, value, length); 
	printResult(value);
	
	if(type == 4)
		d_union = value;
	else if(type == 5)
		f_union = value;
	
	int repeat = ANSWER_YES;
	while(repeat == ANSWER_YES)
	{
		clearLines(coordInversion, yInversion);
						
		gotoxy(0, coordInversion);
		std::cout.precision(17);
		if(type == 4)
		{
			printArray(toBinScale(l_union), true);
			invertBits(l_union);
			std::cout << "\nResult: " << d_union;
		}
		else if(type == 5)
		{
			printArray(toBinScale(i_union), true);
			invertBits(i_union);
			std::cout << "\nResult: " << f_union;
		}
		std::cout << "\nDo invertion again? (y/n) ";
				
		repeat = getch();
	}
	if(type == 4)
		modulation(toBinScale(l_union), l_union);
	else if(type == 5)
		modulation(toBinScale(i_union), i_union);
}

int main() {	
	// int change = 0, num = 0;
	// std::cout <<  "before: change = " << change << ", num = " << num;
	// for(int i = 0; i < 16; i++)
	// 	change += pow(2, i);
	// //change <<= to;
	// __asm
	// {
	// 	MOV eax, change;
	// 	XOR num, eax;
	// }
	// std::cout << "\nafter: change = " << change << ", num = " << num;
	// getch();
	// return 0;

	SetConsoleTextAttribute(hConsole, PRIMARY_COLOR);

	int type = 0, colorMenuType = 0;
	char c; int i; long l; bool b; double d; float f;

	while(true) 
	{
		type = menu(" 1. char\n 2. int\n 3. long\n 4. boolean\n 5. double\n 6. float\n 7. Menu colors\n 8. Audio recording\n");
		if(type == 6) {
			int bia = 8;
			int n = 17, m = 15;
		
			colorMenuType = menu(" 1. Main color\n 2. Secondary color\n");
			
			if(colorMenuType == 2)
				continue;
	
			for(int i = 0; i < 255; i++)
			{
				if(i % (m) == 0 && i != 0)
					std::cout << "\n";
				
				SetConsoleTextAttribute(hConsole, i);
				std::cout << "Example";
				SetConsoleTextAttribute(hConsole, PRIMARY_COLOR);
				std::cout << " ";
			}

			int value = moveCursor(n - 1, m - 1, bia);
			if(value >= 0)
				if(colorMenuType == 0)
					PRIMARY_COLOR = value;
				else
					SECOND_COLOR = value;
					
			SetConsoleTextAttribute(hConsole, PRIMARY_COLOR);
			continue;
		}
		else if(type == 7)
		{
			system("cls");
			drawSound();
		}
		else if(type == 8)
			break;
		
		if(type == 0)
			typenameInput(c, type);
		else if(type == 1)
			typenameInput(i, type);
		else if(type == 2)
			typenameInput(l, type);
		else if(type == 3)
			typenameInput(b, type);
		else if(type == 4)
			doubleFloatInput(d, type);
		else if(type == 5)
			doubleFloatInput(f, type);
		
		std::cout << "\nPress any key to continue...";
		getch();
	}
	
	gotoxy(0, 20);
	return 0;
}
