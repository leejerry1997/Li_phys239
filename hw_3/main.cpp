/**
 * HW 3   File name: main.cpp
 **/
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <valarray>
#include <sciplot/sciplot.hpp>
#include <hw_3/ft.h>

#define STEP 5000    // Number of step to solve the problem
#define Z 1          // Ion charge

using namespace std;

const double K=253.559; // ke^2/m in SI unit
const double XINIT = -5.29177 * pow(10,-9); // initial x of electron
const double QE = 1.6 * pow(10,-19);   // Electron charge
const double C = 3 * pow(10,8); // Speed of light

void graph(const double * const, const double * const,
 const double * const, const double * const,
 const double * const, const double * const,
 const double * const,
 string, const double);

/**
 * Function void p1(double &, double &): Initialize the problem
 * Parameters: y_init - initial position
 *             v_init - initial velocity in x direction
 **/
void p1(double & y_init, double & v_init)
{
	const double a0 = 5.29177E-11;
	y_init = 10 * a0;
	v_init = 10E5;
}

/**
 * Function double * getAcceleration(double * const r, double * a): obtain acceleration of particle at position r
 * Parameters: r - pointer to position
 *             a - pointer to acceleration to be assigned
 * Return: a
 **/
void getAcceleration(const double rx, const double ry, double * a)
{
	double d = sqrt(rx * rx+ry * ry);
	a[0] = -K*Z * rx / pow(d,3);
	a[1] = -K*Z * ry / pow(d,3);
}

/**
 * Function void p2(double &, double &, double &, double &, const double): update position and velocity from time t to time t+delta t
 * Parameters: yInit - impact parameter
 *             vInit - initial velocity
 *             dt    - timestep
 *             FileName - output file name
 **/
void p2(const double yInit, const double vInit, const double t, string FileName)
{
	int i = 0; // Iterator
	double a[2]; // Place to store acceleration
	const double dt = t/(double)STEP;
	
	/* Declaration and initialization of data */
	double x[STEP+1] = {0};
	double y[STEP+1] = {0};
	double vx[STEP+1] = {0};
	double vy[STEP+1] = {0};
	double ax[STEP+1] = {0};
	double ay[STEP+1] = {0};
	
	/* Initial condition */
	x[0] = XINIT;
	y[0] = yInit;
	vx[0] = vInit;
	vy[0] = 0;

	/* Loop around to do integral */
	for(i=0;i<STEP; i++)
	{
		getAcceleration(x[i],y[i],a);
		ax[i] = a[0];
		ay[i] = a[1];
		vx[i+1] = vx[i] + a[0] * dt;
		vy[i+1] = vy[i] + a[1] * dt;
		x[i+1] = x[i] + vx[i] * dt;
		y[i+1] = y[i] + vy[i] * dt;
	}
	
	/* Fourier Transform */
	double acc[STEP] = {0};
	double hata[STEP] = {0};
	double power[STEP] = {0};
	for(i=0;i<STEP;i++)
	{
		acc[i] = sqrt(ax[i]*ax[i]+ay[i]*ay[i]);
	}
	ft(acc,STEP,hata);
	
	for(i=0;i<STEP; i++)
	{
		power[i] = 2 * Z * QE*QE*hata[i]/(3*C*C);
	}
	
	/* Plot */
	graph(x,y,vx,vy,ax,ay,power,FileName, dt);
}

void graph(const double * const x, const double * const y,
			const double * const vx, const double * const vy,
			const double * const ax, const double * const ay,
			const double * const pow,
			string FileName, const double dt)
{
	using namespace sciplot;
	multiplot mp1,mp2,mp3,mp4;
	plot plt1, plt2, plt3, plt4, plt5,plt6;
	valarray <double> xArray(x, STEP);
	valarray <double> yArray(y, STEP);
	valarray <double> vxArray(vx, STEP);
	valarray <double> vyArray(vy, STEP);
	valarray <double> axArray(ax, STEP);
	valarray <double> ayArray(ay, STEP);
	valarray <double> powArray(pow, STEP);
	valarray <double> time(STEP);
	valarray <double> freq(STEP);
	
	/* Initialize time array */
	for(int i=0;i<STEP; i++)
	{
		time[i] = dt * (double)i;
		freq[i] = 2 * M_PI / dt;
	}

	plt1.draw(xArray,yArray).title("Path");
	plt2.draw(time,vxArray).title("Velocity in x");
	plt3.draw(time,vyArray).title("Velocity in y");
	plt4.draw(time,axArray).title("Acceleration in x");
	plt5.draw(time,ayArray).title("Acceleration in y");
	plt6.draw(freq ,powArray).title("Power vs frequency");

	mp1.title("xy-" + FileName);
	mp1.layout(1,1);
	mp1.add(plt1);
	mp1.save("xy_" + FileName + ".pdf");
	
	mp2.title("v-" + FileName);
	mp2.layout(2,1);
	mp2.add(plt2).add(plt3);
	mp2.save("v_" + FileName + ".pdf");
	
	mp3.title("a-" + FileName);
	mp3.layout(2,1);
	mp3.add(plt4).add(plt5);
	mp3.save("a_" + FileName + ".pdf");
	
	mp4.title("power-" + FileName);
	mp4.layout(1,1);
	mp4.add(plt6);
	mp4.save("pow_" + FileName + ".pdf");
}

/**
 * Driver function
 **/
int main()
{
	const double a0 = 5.29177 * pow(10,-11);
	p2(10 * a0, pow(10,6),(-XINIT)/(2*pow(10,5)),"p1"); //for problem 1
	p2(15 * a0, pow(10,6),(-XINIT)/(2*pow(10,5)),"config2");
	p2(30 * a0, pow(10,6),(-XINIT)/(2*pow(10,5)),"config3");
	return 0;
}
