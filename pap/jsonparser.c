#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

char* dataType[] = {"COLON_START", "COLON_END", "STRING_START", 
		"STRING_END", "LIST_START", "LIST_END",
		"INT_START", "INT_END","BOOL_START", 
		"BOOL_END", "NULL" };

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

	FILE* fptr = fopen(argv[1], "r");
	if (!fptr) 
	{ 
		fprintf(stderr,
			"invalid filepath\nusage: parser <filepath.json>\n");
		return -1;
	}

	// obtaining the filesize
	fseek(fptr, 0L, SEEK_END);
	int len = ftell(fptr);

	char* buffer = (char*)malloc(sizeof(char)*len);
	fseek(fptr, 0L, SEEK_SET);

	size_t bytesread = fread(buffer, sizeof(char), len, fptr);
	if (bytesread != len)
	{
		fprintf(stderr,
			"short read of '%s': expected %d bytes\n",
			argv[1], len);
		return -1;
	}

	struct t_storage** container = malloc(sizeof(struct t_storage*) * len);
	int c_len = 0; // container length
	
	bool insideString = false;
	char dest[100];

	for (int i = 0; i <= len; i++)
	{
		switch (buffer[i])
		{
			case '{':
				container[c_len] = createContainer(0, i);
				c_len += 1;
				break;
			case '}':
				container[c_len] = createContainer(1, i);
				c_len += 1;
				break;
			case '\'':
				if (insideString == true)
				{
					container[c_len] = createContainer(3, i);
					insideString = false;
				} else {
					container[c_len] = createContainer(2, i);
					insideString = true;
				}
				c_len += 1;
				break;
			case ' ':
				if (insideString == false) break;
		}
	}

	insideString = false;
	int start = 0;
	for (int i = 0; i < c_len; i++)
	{
		int datatype = container[i]->datatype;
		int pos = container[i]->pos;
		switch(datatype)
		{
			case 2:
				start = pos + 1;
				insideString = true;
				break;
			case 3: // STRING_START
				if (insideString)
				{
					strncpy(dest, buffer + start, pos - start);
					dest[pos-start] = '\0';
					printf("string (%d, %d)-> %s \n",
							start,pos,dest);
					insideString = false;
				}
		}
	}

	free(container);
	return 0;

}
