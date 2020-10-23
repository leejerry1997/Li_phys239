/**
 * HW 2   File name: main.cpp
 **/
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <valarray>
#include <sciplot/sciplot.hpp>

#define LW 4   // Line width of plot
#define N 1000 // Number of steps in integral
#define NArray 1000 // Number of element in array

using namespace std;
ofstream fout("Answers.txt");

//const double ND = 3.085677 * exp(log(10) * 18); // column number density
const double ND = 1; // I use ND=1 for convenience so we can write everything in optical depth w/o translation

/**
 * Answer to problem 2
 * Input: sigma - cross section
 *        I0    - initial intensity
 *        S     - Source f'n
 *
 * Output: intensity at s=D
 */
double p2(const double sigma, const double I0, const double S)
{
	const double tau = ND * sigma; // optical step
	const double ds=tau/N;      // Increment of step
	int i=0;                           // Iterator
	double I=I0;                  // Integrated result, initiallized as I=I0 at tau=0
	for(i=0;i<N; i++)
	{
		I += (S-I) * ds;
	}

	return I;
}

/**
 * Answer to problem 3
 * Input: nu     - pointer to array of frequencies
 *        sigma  - pointer to array of frequency-dependent cross section
 *        sigma0 - maximum of cross section
 **/
void p3(double * const nu, double * const sigma, const double sigma0, double var=NArray/16)
{
	int i = -1; // Iterator
	double dnu = 0.01;
	// Iterate through full array and initialize;
	while(++i < NArray)
	{
		nu[i] = dnu * i;
		sigma[i] = sigma0 * exp(-pow((NArray/2-(double)i)/var,2)/2);
	}

}

/**
 * Answer to problem 4
 * Input: sigma - const pointer to array of frequency-dependent cross section
 *        I     - const pointer to array of frequency-dependent intensity
 *        I0    - initial intensity
 *        S     - Source f'n
 *        FileName - File name of plot
 **/
void p4(const double * const sigma, double * const I, const double I0, const double S)
{
	int i=0;
	for(i=0;i<NArray; i++)
	{
		I[i] = p2(sigma[i], I0, S);
	}
}

void graph(const double * const nu, const double * const sigma, const double * const I, string FileName, const double S)
{
	using namespace sciplot;
	multiplot mp;
	plot plt1, plt2;
	valarray <double> x(nu, NArray);
	valarray <double> y1(sigma, NArray);
	valarray <double> y2(I, NArray);
	valarray <double> s(S, NArray);

	plt1.draw(x,y1).title("sigma");
	plt2.draw(x,s).title("S");
	plt2.draw(x,y2).title("Intensity");

	mp.title(FileName);
	mp.layout(2,1);
	mp.add(plt1).add(plt2);

	mp.save(FileName);
}

/**
 * Driver to problem 4
 *
 **/
void p4Driver()
{
	double nu[NArray] = {0};
	double sigma[NArray] = {0};
	double I[NArray]={0};
	double sigma0 = 1;
	double S = 1;
	double I0=1;
	
	// Part a
	I0=0;
	sigma0=.5;
	p3(nu,sigma,sigma0);
	p4(sigma,I,I0,S);
	graph(nu,sigma,I,"Part_a.pdf", S);
	
	// Part b
	I0=2;
	sigma0=.5;
	p3(nu,sigma,sigma0);
	p4(sigma,I,I0,S);
	graph(nu,sigma,I,"Part_b.pdf", S);
	
	// Part c
	I0=.5;
	sigma0=.5;
	p3(nu,sigma,sigma0);
	p4(sigma,I,I0,S);
	graph(nu,sigma,I,"Part_c.pdf", S);
	
	// Part d
	I0=.5;
	sigma0=100;
	p3(nu,sigma,sigma0,N);
	p4(sigma,I,I0,S);
	graph(nu,sigma,I,"Part_d.pdf", S);

	// Part e
	I0=.5;
	sigma0=20;
	p3(nu,sigma,sigma0);
	p4(sigma,I,I0,S);
	graph(nu,sigma,I,"Part_e.pdf", S);

	// Part f
	I0=2;
	sigma0=20;
	p3(nu,sigma,sigma0);
	p4(sigma,I,I0,S);
	graph(nu,sigma,I,"Part_f.pdf", S);
}

/**
 * Driver function
 **/
int main()
{

	//p3(nu,sigma,sigma0);
	//p4();
	p4Driver();

	return 0;
}
