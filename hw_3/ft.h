#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
#include <string>
#include <valarray>
#include <sciplot/sciplot.hpp>

using namespace std;

/**
 * Input: x - function to be transformed
 *        N - number of samples in function
 *        y - |\hat{x}|^2 after transfomration
 **/
void ft(double * const x, const int N, double * const y)
{
	int i,j;
	double Re,Im;
	for(i=0;i<N;i++)
	{
		Re = Im = 0;
		for(j=0;j<N;j++)
		{
			Re += x[j] * cos(2 * M_PI * (double)i * (double)j/(double)N);
			Im += -x[j] * sin(2 * M_PI * (double)i * (double)j/(double)N);
		}
		y[i] = (Re/(double)N)*(Re/(double)N)+(Im/(double)N)*(Im/(double)N);
	}
}