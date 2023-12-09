//When will the time_t's wrap around?
//Write a program to find out.
// 	1. Look at the definition of time_t. This is in file /usr/include/time.h.
// 	2. Code a program to place the highest value into a variable of type time_t, then 
// 	pass it to ctime() to convert it into an ASCII string. Print the string. Note that 
// 	ctime has nothing to do with the language C, it just means "convert time."
// 	
//For how many years into the future does the anonymous technical writer who removed the
//comment have to worry about being dogged by a UNIX daemon? Amend your program to
//find out.
// 	1. Obtain the current time by calling time().
// 	2. Call difftime() to obtain the number of seconds between now and the highest
// 	value of time_t.
// 	3. Format that value into years, months, weeks, days, hours, and minutes. Print it.
//Is it longer than your expected lifetime?
#include <time.h>
#include <stdio.h>

int main(void)
{
	time_t biggest = 0x7FFFFFFF;
	time_t current_time; 
	// obtaining the current time
	time(&current_time);
	double difference = difftime(biggest, current_time);

	printf("differencetime %s - %s = %.2f\n", 
			ctime(&biggest), ctime(&current_time), difference);

	printf("biggest time %s\n", 
			asctime(gmtime(&biggest)));
	return 0;
}
