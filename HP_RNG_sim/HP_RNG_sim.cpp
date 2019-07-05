// HP_RNG_sim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WIN32_LEAN_AND_MEAN // Meme

#include <conio.h>
#include <iostream>
#include <thread>

// For getchar
#include <stdio.h>

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

		if (temp != 0)
		{
			int ch = _getch();
			if (ch == 'q' && bPause)
			{
				bThreadbreak = true;
			}
			else if (ch == 'p')
			{
				// Toggle pausing
				bPause = !bPause;
			}
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

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cerr << "You need to pass an argument that specifies how many trials to run.\n";
		return -1;
	}

	else
	{
		unsigned long i = 0U;
		unsigned long times = strtoul(argv[1], nullptr, 10);
		std::cout << "Running " << times << " trials. Press \"p\" to pause.\n";
		unsigned long good = 0U;
		int gryff;
		int slyth;

		// Init a thread for pausing
		std::thread pauseThread(GetKBInRunner);

		// Loop until desired
		while (i < times)
		{
			// Infinite loop to pause
			if (bPause)
			{
				std::cout << "Execution paused. Press \"p\" to resume or \"q\" to quit.\n"
					<< "Trials: " << i << "\nGood: " << good << "\n";
				do
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					if (bThreadbreak)
					{
						std::cout << "Execution terminated.";
						pauseThread.join();
						return 0;
					}
				}
				while (bPause);
				std::cout << "Resuming...\n";
			}

			gryff = 30;
			slyth = (int)RandRange(40.0F, 80.0F);
			do
			{
				gryff += (int)RandRange(1.0F, 10.0F);
			}
			while (gryff < slyth);
			if (gryff - slyth <= 5)
			{
				good += 1U;
			}
			i++;
		}

		std::cout << "Execution completed.\nTrials: " << times << "\nGood: " << good;

		// Break the thread
		bThreadbreak = true;

		// Now close it
		pauseThread.join();

		return 0;
	}
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
