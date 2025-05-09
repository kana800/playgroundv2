// THIS IS NOT A COMPLETE JSON PARSER; 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "platform_metrics.h"

char* dataType[] = {"COLON_START","COLON_END","STRING_START", 
		"STRING_END","LIST_START","LIST_END",
		"INT_START","INT_END","BOOL_START", 
		"BOOL_END","SEMI_COLON","COMMA","NULL"};

struct t_storage {
	int datatype;
	int pos;
};

struct t_storage* createContainer(int datatype, int pos)
{
	struct t_storage* t = malloc(sizeof(struct t_storage) * 1);
	t->datatype = datatype;
	t->pos = pos;
	return t;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr,"usage: parser <filepath.json>\n");
		return -1;
	}

	TimeBlock();
	
	FILE* fptr = fopen(argv[1], "r");
	if (!fptr) 
	{ 
		fprintf(stderr,
			"invalid filepath\nusage: parser <filepath.json>\n");
		return -1;
	}

	EndTimer();

	u64 OSStart = ReadOSTimer();
	u64 CPUStart = ReadCPUTimer();
	u64 OSEnd, OSElapsed = 0;

	BeginTimer("Parser");

	int bufferlen = 75;
	char buffer[bufferlen];
	
	// skipping the first two lines
	fgets(buffer, bufferlen, fptr);
	fgets(buffer, bufferlen, fptr);

	double data[4]; // x0, y0, x1, y1
	char* delim = ":";

	int count = 0;
	while (fgets(buffer, bufferlen, fptr))
	{
		BeginTimer("ForLoop");
		char dest[100];
		int toread = 8;
		char* token = strtok(buffer, delim);
		while (token != NULL)
		{
			int len = strlen(token);
			switch (count)
			{
				case 1: 
				case 2:
				case 3:
					memcpy(dest, token, len - 5);
					dest[len - 4] = '\0';
					//printf("%d\t%s\n",count, dest);
					data[count - 1] = atof(dest);
					break;
				case 4:
					if (token[len - 2] != ',')
					{
						memcpy(dest, token, len - 2);
						dest[len - 1] = '\0';
					} else 
					{
						memcpy(dest, token, len - 3);
						dest[len - 2] = '\0';
					}
					data[count - 1] = atof(dest);
					//printf("%d\t%s\n",count, dest);
					break;
			}
			//printf("(%d) %s %d \n", count, token, len);
			//printf("(%d %s)\n---\n",count, dest);
			count += 1;
			token = strtok(NULL, delim);
		}
		count = 0;
		for (int i = 0; i < 4; i++)
		{
			printf("%d %lf\n",i, data[i]);
		}
		EndTimer();
	}


	OSEnd = ReadOSTimer();
	u64 CPUEnd = ReadCPUTimer();
	u64 CPUElapsed = CPUEnd - CPUStart;
	OSElapsed = OSEnd - OSStart;
	EndTimer();
	
	printf("Parsing: %llu %llu\n", OSElapsed, CPUElapsed);

	return 0;
}
