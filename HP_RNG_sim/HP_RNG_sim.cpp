#define WIN32_LEAN_AND_MEAN // Meme

#include <Windows.h>
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

static int i = 0;
static int imin;
static int imax;
int* results;

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
	return i / (float)RAND_MAX;
}

inline float RandRange(float Min, float Max)
{
	return Min + (Max - Min) * appFrand();
}

int ParseArg(int* iout, const char* arg, const char* errmsg = (char *)nullptr, std::logic_error* chout = (std::logic_error *)nullptr)
{
	try
	{
		std::string str = arg;
		*iout = std::stoi(str);
		return 0;
	}
	catch (std::logic_error ex)
	{
		if (chout)
		{
			*chout = ex;
		}
		if (errmsg)
		{
			std::cerr << errmsg;
		}
		return -1;
	}
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		std::cerr << "You must specify two arguments, a min and max to test respectively.";
		return -1;
	}

	if (
		ParseArg(&imin, argv[1], "Argument 1 is invalid. It must be an integer.") ||
		ParseArg(&imax, argv[2], "Argument 2 is invalid. It must be an integer.")
		)
	{
		return -1;
	}

	if (imin >= imax) {
		std::cerr << "Argument 1 must be less than argument 2.";
		return -1;
	}

	float fmin = (float)imin;
	float fmax = (float)imax;

	int size = imax - imin + 1;
	results = new int[size] {};
	//ZeroMemory(results, size * sizeof(int));

	std::cout << "Running " << (RAND_MAX + 1) << " trial";
	if (imax != 1U) {
		std::cout << "s";
	}
	std::cout << ". Press \"p\" to pause.\n";

	// Init a thread for pausing
	std::thread pauseThread(GetKBInRunner);

	auto start = std::chrono::high_resolution_clock::now();
	// Loop until desired
	while (i <= RAND_MAX)
	{
		// Infinite loop to pause
		if (bPause)
		{
			std::cout << "Execution paused. Press \"p\" to resume or \"q\" to quit.\n"
				<< "Trials: " << i << "\n";
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
		int index = (int)RandRange(fmin, fmax) - imin;
		results[index] = results[index] + 1;
		i++;
	}
	auto timeTaken = std::chrono::high_resolution_clock::now() - start;
	long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(timeTaken).count();
	double seconds = millis / 1000.0;
	std::cout << "Execution completed.\nTrials: " << (i-1) << "\n"
		<< "Time elapsed: " << seconds << " seconds\n";

	if (results != nullptr)
	{
		for (int i = imin; i <= imax; i++)
		{
			std::cout << i << ": " << results[i - imin] << "\n";
		}
	}

	delete[] results;

	// Break the thread
	bThreadbreak = true;

	// Now close it
	pauseThread.join();

	return 0;
}
