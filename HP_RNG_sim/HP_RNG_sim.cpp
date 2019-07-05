#define WIN32_LEAN_AND_MEAN // Meme

#include <string>
#include <conio.h>
#include <iostream>
#include <thread>
#include <stdio.h>
#include <time.h>

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
		std::cerr << "You need to pass a number an argument that specifies how many trials to run.\n";
		return -1;
	}

	else
	{
		unsigned long times = 0U;
		try
		{
			std::string str = argv[1];
			times = stoul(str, NULL, 10);
		}
		catch (std::invalid_argument ex)
		{
			std::cerr << "You need to pass a number as an argument that specifies how many trials to run.\n";
			return -1;
		}
		catch (std::out_of_range ex)
		{
			std::cerr << "The value " << argv[1] << " exceeds the maximum value of " << ULONG_MAX << ".\n";
			times = ULONG_MAX;
		}

		unsigned long i = 0U;
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
			while (gryff <= slyth);
			if (gryff - slyth <= 5)
			{
				good++;
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
