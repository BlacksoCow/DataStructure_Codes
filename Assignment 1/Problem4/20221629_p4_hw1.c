#include <stdio.h>
#define MAXLEN 100
int Len(char string[])
{ // Return the length of the string
	int idx = 0;
	while (string[idx++] != '\0');
	return idx - 1;
}
void fail(char pat[], int failure[])
{ // Initialize failure array
	int i = 1, j = 0;
	failure[0] = -1;
	while (i < Len(pat))
	{
		if (pat[i] == pat[j])
			failure[i++] = j++;
		else if (j == 0)
			failure[i++] = -1;
		else
			j = failure[j - 1] + 1;
	}
	return;
}
void pmatch_all(char string[], char pat[], int failure[])
{ // Use KMP algorithm to print pat's starting index
	int i = 0, j = 0, slen = Len(string), plen = Len(pat);
	while (i < slen)
	{
		if (string[i] == pat[j])
		{
			i++; j++;
			if (j == plen) // If pat found, print index
				printf("%d\n", i - plen);
		}
		else if (j == 0)
			i++;
		else
			j = failure[j - 1] + 1;
	}
	return;
}
void GetString(char string[])
{ // If last char is '\n', replace to '\0'
	fgets(string, MAXLEN + 1, stdin);
	if (string[Len(string) - 1] == '\n')
		string[Len(string) - 1] = '\0';
	else
		while (getchar() != '\n');
	return;
}
int main(int argc, char** argv)
{
	char str[MAXLEN + 1], pat[MAXLEN + 1];
	int failure[MAXLEN + 1];
	GetString(str);
	GetString(pat);
	fail(pat, failure);
	pmatch_all(str, pat, failure);
	return 0;
}