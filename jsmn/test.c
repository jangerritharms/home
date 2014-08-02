#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsmn.h"

#define MAX_TOKENS 512

struct color_theme{
	char *REQ[] = {"normal, }
}

typedef enum {START, STOP, KEY, VALUE, SKIP, PRINT, PROCESS} parse_state;

char *REQ[] = {"dies", "normal", "colors", NULL};

static char * get_name(char * js, jsmntok_t * t);
static char * file_to_string(const char *);
static void get_json_data(char *[]);

int main(int argc, const char *argv[])
{
	get_json_data(REQ);
	return 0;
}

static void get_json_data(char * REQUIRED[])
{
	char *js;
	int r;
	jsmn_parser p;
	jsmntok_t tokens[MAX_TOKENS];
	int i;
	int j;
	int k;
	unsigned int object_tokens;
	parse_state state = START;

	js = file_to_string("/home/jan/.config/colors/solarized_light.json");

	jsmn_init(&p);
	r = jsmn_parse(&p, js, strlen(js), tokens, 128);
	if (r < 0 ) printf("Parsing of file failed"),exit(EXIT_FAILURE);

	for (i = 0, j = 1; j > 0; i++, j--) {
		jsmntok_t *t = &tokens[i];

		if (t->start == -1 || t->end == -1) printf("Wrong token"),exit(EXIT_FAILURE);
		if (t->type == JSMN_ARRAY || t->type == JSMN_OBJECT)
			j += t->size;

		switch (state) {
			case START:
				if (t[0].type != JSMN_OBJECT) 
					printf("Root element must be object"),exit(EXIT_FAILURE);

				state = KEY;
				object_tokens = t->size;
				
				if (object_tokens == 0) state = STOP;
				if (object_tokens % 2 != 0)
					printf("Object must have even number of children"), exit(
							EXIT_FAILURE);
				
				break;
			case KEY:
				object_tokens--;
				
				if (t->type != JSMN_STRING)
					printf("Object key must be string"), exit(EXIT_FAILURE);

				state = SKIP;
				
				while (REQUIRED[k++] != NULL){
					if (strcmp(get_name(js, t), REQUIRED[k-1])==0){
						printf("%s: ", REQUIRED[k-1]);
						if (tokens[i+1].type == JSMN_STRING || tokens[i+1].type == JSMN_PRIMITIVE)
							state = PRINT;
						else
							state = PROCESS;
						break;
					}
				}
				k=0;
				break;
			case SKIP:
				if (t->type != JSMN_STRING && t->type != JSMN_PRIMITIVE)
					printf("Object value must be string or primitive"), exit(EXIT_FAILURE);

				object_tokens--;
				state = KEY;

				if (object_tokens == 0)
					state = STOP;

				break;
			case PRINT:
				puts(get_name(js, t));

				object_tokens--;
				state=KEY;

				if (object_tokens == 0)
					state = STOP;
				
				break;
			
			case PROCESS:{
				object_tokens += t->size;
				puts(get_name(js, t));

				object_tokens--;
				state=KEY;

				if (object_tokens == 0)
					state = STOP;
				
				}
				break;
			



			case STOP:
				break;

			default:
				printf("Invalid state %u", state);
		}
	}

	free(js);
}

/**
 * Gets key of token
 */
static char * get_name(char * js, jsmntok_t * t){
	js[t->end] = '\0';
	return js + t->start;
}

/**
 * Creates a string from a file
 */
static char * file_to_string(const char * fn){
	/* Read file to string */
	FILE *fp;
	long lSize;
	char *js;

	fp = fopen(fn, "r");
	if (!fp) perror("Error opening file."), exit(1);
	fseek (fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);
	js = calloc(1, lSize+1);
	if (!js) fclose(fp),fputs("Memory allocation failed", stderr), exit(1);
	if (1!=fread(js, lSize, 1, fp))
		fclose(fp), fputs("Reading of file failed", stderr), exit(1);
	fclose(fp);

	return js;
}
