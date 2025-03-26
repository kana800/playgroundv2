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
	struct t_storage* key;
	int v_datatype;
	int v_pos;
	struct t_storage* value;
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
	struct t_keyvalue** keyvaluearr = malloc(sizeof(struct t_keyvalue*) * len);
	
	int c_len = 0; // container length
	
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

	bool insidestring = false;
	bool insidelist = false;
	char dest[100];

	for (int i = 0; i <= len; i++)
	{
		switch (buffer[i])
		{
			case '{':
				container[c_len] = createContainer(0, i);
				colon_opening_arr[co_c] = i;
				co_c += 1;
				c_len += 1;
				break;
			case '}':
				container[c_len] = createContainer(1, i);
				colon_closing_arr[cc_c] = i;
				cc_c += 1;
				c_len += 1;
				break;
			case '\'':
				if (insidestring == true) // STRING END
				{
					container[c_len] = createContainer(3, i);
					insidestring = false;
				} else {		 // STRING START
					container[c_len] = createContainer(2, i);
					insidestring = true;
				}
				c_len += 1;
				break;
			case ':':
				if (insidestring == false)
					container[c_len] = createContainer(10, i);
				c_len += 1;
				break;
			case '[':
				container[c_len] = createContainer(4, i);
				list_opening_arr[lo_c] = i;
				lo_c += 1;
				c_len += 1;
				break;
			case ']':
				container[c_len] = createContainer(5, i);
				list_closing_arr[lc_c] = i;
				lc_c += 1;
				c_len += 1;
				break;
			case ' ':
				if (insidestring == false) break;
		}
	}

	assert(lc_c == lo_c);
	assert(cc_c == co_c);

	insidestring = false;

	int startpos = 0;
	int prevpos = 0;
	
	struct t_storage* prev_container = NULL;

	for (int i = 0; i < c_len; i++)
	{
		int datatype = container[i]->datatype;
		int currpos = container[i]->pos;

		switch(datatype)
		{
			case 2: // STRING_START
				startpos = currpos + 1;
				insidestring = true;
				break;
			case 3: // STRING_END
				if (insidestring == true)
				{
//					strncpy(dest, buffer + start, pos - start);
					memcpy(dest, buffer+startpos, currpos-startpos);
					dest[currpos-startpos] = '\0';
					fprintf(stdout, "string (%d, %d)-> %s \n",
							startpos,currpos,dest);
					insidestring = false;
				}
				break;
			case 10: // SEMI COLON':'
//				assert(prev_container->datatype != 4);
				if (prev_container->datatype != 3) goto datatypeerr;
				fprintf(stdout,"%s:%s\n",
					dataType[prev_container->datatype],
					dataType[datatype] );
				break;
		}
		prev_container = container[i];
	}


	free(container);
	free(keyvaluearr);
	free(list_opening_arr);
	return 0;

datatypeerr:
	fprintf(stderr, "KEY value should be STRING TYPE\n");
	return -1;
}
