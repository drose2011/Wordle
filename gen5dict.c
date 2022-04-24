#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUF_SIZE 4096

int main()
{
	char buf[BUF_SIZE];
	FILE *dict = fopen("dict_2.txt", "r");
	FILE *dict5 = fopen("dict5.txt", "w");
	while(fgets(buf, BUF_SIZE, dict) != NULL) {
		if(strlen(buf) == 6 && islower(buf[0])) {
			fwrite(buf, 1, 6, dict5);
		}
	}
	fclose(dict); fclose(dict5);
}
