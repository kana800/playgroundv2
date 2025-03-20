#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* dataType[] = { "KEY", "VALUE", "STRING", 
		     "INT", "BOOL", "JSON", 
		     "ARRAY", "NULL" };

//typedef struct __jsondata__
//{
//	int datatype;
//	void* databucket;
//} value;
//
//typedef struct __jsonkey__
//{
//	// ptr to string
//	char* key;
//	// ptr to json data
//	jsonData* value;
//} key;

// jsonData* createjsonDataStructure(int datatype, )
// {
// 	jsonData* ptr = (jsonData*)malloc(sizeof(value));
// 	ptr->datatype = datatype;
// 	switch (datatype)
// 	{
// 		case 0: // key type
// 		case 1: // value
// 		case 2: // string 
// 		case 3: // integer
// 		case 5: 
// 	}
// 	return ptr;
// }


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
		fprintf(stderr,"invalid filepath\nusage: parser <filepath.json>\n");
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
		fprintf(stderr,"short read of '%s': expected %d bytes\n", argv[1], len);
		return -1;
	}

	fprintf(stdout, "\n%d\n%s\n%c",len,buffer, buffer[1]);
	return 0;

}
