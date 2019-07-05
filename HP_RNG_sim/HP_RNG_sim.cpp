// HP_RNG_sim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#define TIMES 500000U //set this to how many trials you want to perform

//int RandRange(int Min, int Max) {
//	int Range = Max - Min;
//	int R = Range > 0 ? rand() % Range : 0;
//	return R + Min;
//}

inline float appFrand() {
	return rand() / (float)RAND_MAX;
}

inline float RandRange(float Min, float Max) {
	return Min + (Max - Min) * appFrand();
}

int main()
{
	unsigned int good = 0U;
	int gryff;
	int slyth;

	for (unsigned int i = 0U; i < TIMES; i++)
	{
		gryff = 30;
		slyth = (int)RandRange(40.0F, 80.0F);
		while (gryff < slyth)
		{
			gryff += (int)RandRange(1.0F, 10.0F);
		}
		if (gryff - slyth <= 5)
		{
			good += 1U;
		}
		std::cout << "trial #" << (i+1) << ": \n"
			<< "good: " << good << "\n";
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
