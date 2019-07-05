// HP_RNG_sim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WIN32_LEAN_AND_MEAN // Meme

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <thread>

// For getchar
#include <stdio.h>

#define TIMES 1000

static bool bThreadbreak = false;
static bool bPause = false;

// Thread function for pausing
void GetKBInRunner()
{
	while (!bThreadbreak)
	{
		int temp = _kbhit();

		// Program ended
		//if (temp == EOF)
			//break;

		if (temp != 0 && _getch() == 'p')
		{
			// Toggle pausing
			bPause = !bPause;
		}
	}
}

// UE functions. Inlined for preformance reasons
inline float appFrand()
{
	return rand() / (float)RAND_MAX;
}

inline float RandRange(float Min, float Max)
{
	return Min + (Max - Min) * appFrand();
}

int main()
{
	unsigned int times = 0U;
	unsigned int good = 0U;
	int gryff;
	int slyth;

	// Set the position of the cursor to "clear" the screen without having
	// C++ init a new buffer each time
	COORD newPos;
	newPos.X = 0;
	newPos.Y = 0;

	// Init a thread for pausing
	std::thread pauseThread(GetKBInRunner);

	// Loop until desired
	while (times < TIMES)
	{
		// Infinite loop to pause
		while (bPause) 
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		gryff = 30;
		slyth = (int)RandRange(40.0F, 80.0F);
		do
		{
			gryff += (int)RandRange(1.0F, 10.0F);
		} while (gryff < slyth);
		if (gryff - slyth <= 5)
		{
			good += 1U;
		}
		times++;

		// "Clear" the screen
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);

		// Now print
		std::cout << "Trial: " << times << "\t " << "Number of good runs: " << good << "\n";
	}

	// Break the thread
	bThreadbreak = true;

	// Now close it
	pauseThread.join();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
