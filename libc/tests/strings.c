#include <stdio.h>
#include <string.h>

void strcatTest() {
	char str[80];
	strcpy(str,"these ");
	strcat(str,"strings ");
	strcat(str,"are ");
	strcat(str,"concatenated.");
	puts(str);
}

void strcspnTest() {
	char str[] = "shaun73";
	char keys[] = "1234567890";
	int i;
	printf("The first number in '%s'", str);
	i = strcspn(str,keys);
	printf(" is at position %d.\n",i);
}

void strncpyTest() {
	char str1[]= "To be or not to be";
	char str2[6];
	strncpy (str2,str1,5);
	str2[5]='\0';
	puts (str2);
}

void strncatTest() {
	char str1[20];
	char str2[20];
	strcpy(str1,"To be ");
	strcpy(str2,"or not to be");
	strncat(str1, str2, 8);
	puts(str1);
}

void strncmpTest() {
	char str[][5] = { "R2D2" , "C3PO" , "R2A6", "AR25", "R3RE" };
	int n;
	puts("Looking for R2 astromech droids...");
	for (n=0; n<5; n++) {
		if (strncmp(str[n],"R2",2) == 0) {
			printf("found %s\n",str[n]);
		}
	}
}

void strpbrkTest() {
	char str[] = "This is a sample string";
	char key[] = "aeiou";
	char * pch;
	printf ("Vowels in '%s': ",str);
	pch = strpbrk (str, key);
	while (pch != NULL) {
		printf ("%c " , *pch);
		pch = strpbrk (pch+1,key);
	}
	printf ("\n");
}

void strrchrTest() {
	char str[] = "This is a sample string";
	char *pch;
	pch = strrchr(str, 's');
	printf("Last occurrence of 's' in '%s' found at %d (%d) \n", str, pch-str+1, pch);
}

void strchrTest() {
	char str[] = "This is a sample string";
	char * pch;
	printf("Looking for the 's' character in \"%s\"...\n",str);
	pch = strchr(str,'s');
	while (pch!=NULL) {
		printf ("found at %d\n",pch-str+1);
		pch=strchr(pch+1,'s');
	}
}

void strspnTest() {
	int i;
	char strtext[] = "129th";
	char cset[] = "1234567890";

	i = strspn(strtext,cset);
	printf ("The length of initial number in '%s' is %d.\n", strtext, i);
}

void strstrTest() {
	char str[] = "This is a simple string";
	char *pch;
	pch = strstr(str, "simple");
	strncpy(pch, "sample",6);
	puts(str);
}

void strtokTest() {
	char str[] ="- This, a sample string.";
	char * pch;
	printf("Splitting string \"%s\" into tokens:\n",str);
	pch = strtok(str," ,.-");
	while (pch != NULL) {
		printf("%s\n",pch);
		pch = strtok(NULL, " ,.-");
	}
	 
	printf("\n%s\n", str);
}

int main () {
	printf("strcat:\n");
	strcatTest();

	printf("\nstrcspn:\n");
	strcspnTest();

	printf("\nstrncpy:\n");
	strncpyTest();

	printf("\nstrncat:\n");
	strncatTest();

	printf("\nstrncmp:\n");
	strncmpTest();

	printf("\nstrpbrk:\n");
	strpbrkTest();

	printf("\nstrrchr:\n");
	strrchrTest();

	printf("\nstrchr:\n");
	strchrTest();

	printf("\nstrspn:\n");
	strspnTest();

	printf("\nstrstr:\n");
	strstrTest();
	
	printf("\nstrtok:\n");
	strtokTest();

	return 0;
}