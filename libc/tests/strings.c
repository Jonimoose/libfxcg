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
    i = strcspn(str,keys);
    printf("The first number in '%s' is at position %d.\n", str, i);
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

void memccpyTest() {
    const char *src = "This is the sourc";
    char dest[50];
    char *ptr;
    ptr = (char *)(memccpy((void*)dest, (const void*)src, (int)'c', (size_t)(strlen(src)-1)));
    if (ptr) {
        *ptr = '\0';
        printf("The character was found: %s\n", dest);
    } else {
        printf("The character wasn't found\n");
    }
}

void memchrTest() {
    char * pch;
    char str[] = "Examle string";
    pch = (char*) memchr (str, 'p', strlen(str));
    if (pch!=NULL) {
        printf ("'p' found at position %d.\n", pch-str+1);
    } else {
        printf ("'p' not found.\n");
    }
}

size_t max(size_t s1, size_t s2) {
    return s1 > s2 ? s1 : s2 ;
}

void memcmpTest() {
    char str1[256] = "shaun1";
    char str2[256] = "shaun2";
    int n;
    //printf ("Enter a sentence: "); gets(str1);
    //printf ("Enter another sentence: "); gets(str2);
    n=memcmp ( str1, str2,  max(strlen(str1), strlen(str2)));
    if (n>0) printf ("'%s' is greater than '%s'.\n",str1,str2);
    else if (n<0) printf ("'%s' is less than '%s'.\n",str1,str2);
    else printf ("'%s' is the same as '%s'.\n",str1,str2);
}

void memcpyTest() {
    char str1[]="Sample string";
    char str2[40];
    char str3[40];
    char* str4;
    memcpy(str2,str1,strlen(str1)+1);
    str4 = (char*)memcpy(str3,"copy successful",4);

    printf("str1: %s\nstr2: %s\nstr3: %s\nstr4: %s\n",str1,str2,str3,str4);
}

void memmoveTest() {
    char str[] = "memmove can be very useful......";
    memmove(str+20,str+15,11);
    puts(str);
}

void memsetTest() {
    char str[] = "almost every programmer should know memset!";

    puts((char*)memset(str,'-',6));
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

    printf("\nmemccpy:\n");
    memccpyTest();

    printf("\nmemchr:\n");
    memchrTest();

    printf("\nmemcmp:\n");
    memcmpTest();

    printf("\nmemcpy:\n");
    memcpyTest();

    printf("\nmemmove:\n");
    memmoveTest();

    printf("\nmemset:\n");
    memsetTest();

    return 0;
}
