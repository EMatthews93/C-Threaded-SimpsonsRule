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

//function to take the integral of
double f(double x)
{
	return 4.0/(1.0+x*x);
}

//function for calculating the integral of a function
//param: the thread number of the current thread doing calculations
//param: the function to take the integral of
double simpson(int threadNum, double(*func)(double))
{
	double approx = 0.0;
	double h = 1.0/32000.0;
	double multiplier = 0.0;

	double i = 0.0;
	int k = 0;

	//split 32000 slices into 4 equal parts and designate a section to each thread
	for (i = 8000.0*threadNum; i < 8000.0*(threadNum+1.0); i += 1.0)
	{
		//find correct coefficient
		if (k == 0 && threadNum == 0)
			multiplier = 1.0;
		else if (k % 2 == 0)
			multiplier = 2.0;
		else
			multiplier = 4.0;

		approx += multiplier * func(i/32000.0);
		k++;
	}

	if (threadNum == 3) //if it's the last thread, do the last func call
	{
		approx += func(1);
	}

	return (h/3.0)*approx;
}

int main()
{
	FILE* childStreams[4];
	FILE* myOutputStream;

	int fd[4][2];
	int myFd;
	int childPid[4] = {1,1,1,1};
	int parentID = getpid();
	int threadNumber = 0;

	//create 4 children threads to do calculations
	int i;
	for (i = 0; i < 4; i++)
	{
		pipe(fd[i]); //open pipe
		childPid[i] = fork(); //create child

		if (childPid[i] == 0) //if is child
		{
			close(fd[i][0]); //close the input side of the child
			myFd = fd[i][1];
			break;
		}
		else //if is parent
		{
			close(fd[i][1]); //close the output side
			threadNumber++;
		}
	}

	//if I'm a child thread
	if (getpid() != parentID)
	{	
		double avg, sum = 0.0;

		//do 100000 calculations and take the average
		for (i = 0; i < 100000; ++i)
		{
			sum += simpson(threadNumber, f);
		}

		avg = sum / 100000.0;
		
		//write the average to a pipe that's linked to the parent
		write(myFd, &avg, sizeof(double));
	}
	else //is parent, read all children results
	{
		int stat;
		double result = 0.0;

		//for every child thread
		for (i = 0; i < 4; ++i)
		{
			double childResult;

			//read child result from the pipe
			read(fd[i][0], &childResult, sizeof(double));

			//add all results together to get final result
			result += childResult;
		}

		//output the final result
		printf("Result (using threads): %lf\n", result);
		fflush(stdout);
	}

	return 0;
}