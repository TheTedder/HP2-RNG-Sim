#define WIN32_LEAN_AND_MEAN // Meme

#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <chrono>
#include <conio.h>
#include <stdio.h>
#include <time.h>

//historical accuracy
#define RAND_MAX 0x7fff

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
	srand((unsigned)time(NULL)); //do this bcz UE does this
	if (argc == 1)
	{
		std::cerr << "You need to pass a number as an argument that specifies how many trials to run.";
		return -1;
	}

	unsigned long times = 0U;
	try
	{
		std::string str = argv[1];
		times = std::stoul(str, NULL, 10);
	}
	catch (std::invalid_argument)
	{
		std::cerr << "You need to pass a number as an argument that specifies how many trials to run.";
		return -1;
	}
	catch (std::out_of_range)
	{
		std::cerr << "The value " << argv[1] << " exceeds the maximum value of " << ULONG_MAX << ".\n";
		times = ULONG_MAX;
	}

	int gryff_start = 30;

	if (argc > 2)
	{
		try
		{
			std::string str = argv[2];
			gryff_start = std::stoi(str, NULL, 10);
		}
		catch (std::invalid_argument)
		{
			std::cerr << "You need to pass a number as an argument that specifies how many house points gryffindor starts with.";
			return -1;
		}
		catch (std::out_of_range)
		{
			std::cerr << "The value " << argv[2] << " exceeds the maximum value of " << INT_MAX << ".\n";
			gryff_start = INT_MAX;
		}
	}

	unsigned long i = 0U;
	std::cout << "Running " << times << " trial";
	if (times != 1U) {
		std::cout << "s";
	}
	std::cout << ". Press \"p\" to pause.\n";
	unsigned long good = 0U;
	int gryff;
	int slyth;

	// Init a thread for pausing
	std::thread pauseThread(GetKBInRunner);

	auto start = std::chrono::high_resolution_clock::now();
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

		gryff = gryff_start;
		slyth = (int)RandRange(40.0F, 80.0F);
		while (gryff <= slyth)
		{
			gryff += (int)RandRange(1.0F, 10.0F);
		}
		if (gryff - slyth <= 5)
		{
			good++;
		}
		i++;
	}
	auto timeTaken = std::chrono::high_resolution_clock::now() - start;
	long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(timeTaken).count();
	double seconds = millis / 1000.0;
	std::cout << "Execution completed.\nTrials: " << times << "\nGood: " << good << "\n"
		<< "Time elapsed: " << seconds << " seconds";

	// Break the thread
	bThreadbreak = true;

	// Now close it
	pauseThread.join();

	return 0;
}
