/*
Name: Eric Matthews
Student ID: 103601999
Assignment #1
Date: January 28th, 2014
Class: 03-60-330-01
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

//The function to take the integral of
double f(double x)
{
	return 4/(1+x*x);
}

//Simpson's rule for calculating an integral between 0 and 1 using 32000 slices
//takes a function pointer as the function to take the integral of
double simpson(double(*func)(double))
{
	double approx = 0.0;
	double h = 1.0/32000.0;
	int multiplier = 0;

	double i = 0.0;
	int k = 0;

	//use 32000 slices with appropriate coefficients to calculate integral
	for (i = 0.0; i < 1.0; i += 1.0/32000.0)
	{
		//find correct coefficient
		if (k == 0)
			multiplier = 1;
		else if (k % 2 == 0)
			multiplier = 2;
		else
			multiplier = 4;

		approx += multiplier * func(i);
		k++;
	}

	//this is the last call for i == 1
	approx += func(1);

	//return result
	return (h/3.0)*approx;
}

int main()
{

	int i;
	double avg, sum = 0.0;

	//calculate the integral 100000 times and take the average
	for (i = 0; i < 100000; ++i)
	{
		sum += simpson(f);
	}

	avg = sum / 100000.0;

	//output the average
	printf("Result: %lf\n", avg);
	
	return 0;
}
