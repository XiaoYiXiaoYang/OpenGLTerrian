#include <stdio.h>

#include <malloc.h>

#include <string.h>  

char *textFileRead(char *fn)

{

	FILE *fp;

	char *content = NULL;

	int count = 0;

	if (fn != NULL)

	{

		///打开文件 

		fopen_s(&fp, fn, "rb");

		if (fp != NULL)

		{

			fseek(fp, 0, SEEK_END);

			count = ftell(fp);

			rewind(fp);

			if (count > 0)

			{

				content = (char *)malloc(sizeof(char) * (count + 1));

				count = fread(content, sizeof(char), count, fp);

				content[count] = '\0';

			}

			fclose(fp);

		}

	}

	return content;

}

int textFileWrite(char *fn, char *s)

{

	FILE *fp;

	int status = 0;

	if (fn != NULL)

	{

		///打开文件 

		fopen_s(&fp, fn, "w");

		//fp = fopen(fn, "w");

		if (fp != NULL)

		{

			if (fwrite(s, sizeof(char), strlen(s), fp) == strlen(s))

				status = 1;

			fclose(fp);

		}

	}

	return(status);

}