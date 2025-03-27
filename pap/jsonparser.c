#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

char* dataType[] = {"COLON_START", "COLON_END", "STRING_START", 
		"STRING_END", "LIST_START", "LIST_END",
		"INT_START", "INT_END","BOOL_START", 
		"BOOL_END", "SEMI_COLON", "NULL" };


struct t_storage {
	int datatype;
	int pos;
};

struct t_keyvalue {
	int k_datatype;
	int k_pos;
	void* key;
	int v_datatype;
	int v_pos;
	void* value;
};

struct t_keyvalue* createKeyValueContainer(
	char* buffer, int keystartpos, int keyendpos)
{
	// keys are always string type
//	memcpy(dest, buffer+startpos, currpos-startpos);
//	dest[currpos-startpos] = '\0';
}

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

	struct t_keyvalue* keyvaluearr  = malloc(sizeof(struct t_keyvalue) * 100);
	int kv_index = 0;

	struct t_storage** container = malloc(sizeof(struct t_storage*) * len);
	int c_index = 0;
	
	// records the position of the opening bracket 
	// and the closing bracket of the json string
	int* list_opening_arr = malloc(sizeof(int) * len); 
	int* list_closing_arr = malloc(sizeof(int) * len);
	int lo_c = 0; // list opening bracket count
	int lc_c = 0; // list closing bracket count

	int* colon_opening_arr = malloc(sizeof(int) * len);
	int* colon_closing_arr = malloc(sizeof(int) * len);
	int co_c = 0; // colon opening bracket count
	int cc_c = 0; // colon closing bracket count

	int* string_pair_arr = malloc(sizeof(int) * len);
	int sp_c = 0;

	bool insidestring = false;
	bool insidelist = false;
	char dest[100];

	int buffer_index = 0;

	for (int i = 0; i <= len; i++)
	{
		char s = buffer[i];

		switch (s)
		{
			case '{':
				container[c_index] = createContainer(0, i);
				c_index += 1;
				colon_opening_arr[co_c] = i;
				co_c += 1;
				break;
			case '}':
				container[c_index] = createContainer(1, i);
				c_index += 1;
				colon_closing_arr[cc_c] = i;
				cc_c += 1;
				break;
			case '\'':
				if (insidestring == true)
				{
					string_pair_arr[sp_c] = i;
					container[c_index] = createContainer(3, i);
					sp_c += 1;
					insidestring = false;
				} else {
					container[c_index] = createContainer(2, i);
					insidestring = true;	
				}
				c_index += 1;
				break;
			case ':':
				if (insidestring == false)
				{
					container[c_index] = createContainer(4, i);
					c_index += 1;
					break;
				}
			case '[':
				if (insidestring == false)
				{
					container[c_index] = createContainer(4, i);
					list_opening_arr[lo_c] = i;
					c_index += 1;
					lo_c += 1;
					break;
				}
			case ']':
				if (insidestring == false)
				{
					container[c_index] = createContainer(4, i);
					list_closing_arr[lc_c] = i;
					c_index += 1;
					lc_c += 1;
					break;
				}
			case ' ':
				if (insidestring == false) break;
		}
	}

	// counts of list/colon opening 
	// brackets and closing brackets
	assert(lc_c == lo_c);
	assert(cc_c == co_c);

#if DEBUG
	for (int i = 1, j = 0; i < cc_c; i++, j++)
	{
		int a = colon_opening_arr[i];
		int b = colon_closing_arr[j];
		fprintf(stdout, "(%d)%c-(%d)%c\n", a, buffer[a], b, buffer[b]);
	}

	for (int i = 0; i < lc_c; i++)
	{
		int a = list_opening_arr[i];
		int b = list_closing_arr[i];
		fprintf(stdout, "(%d)%c-(%d)%c\n", a, buffer[a], b, buffer[b]);
	}
#endif

	for (int i = 0; i < c_index; i++)
	{
		int datatype = container[i]->datatype;
		int pos  = container[i]->pos;

		fprintf(stdout, "T:%s P:%d\n", dataType[datatype], pos);
	}

	free(string_pair_arr);
	free(colon_opening_arr);
	free(colon_closing_arr);
	free(list_opening_arr);
	free(list_closing_arr);
	return 0;

datatypeerr:
	fprintf(stderr, "KEY value should be STRING TYPE\n");
	return -1;
}
