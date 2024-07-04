#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *findMaxCommonSubstring(char **strings, int numStrings) {
    char *maxSubstring = NULL;
    int maxSubstringLen = 0;
    int str1Len = strlen(strings[0]);

    for (int start = 0; start < str1Len; start++) {
        for (int end = start; end < str1Len; end++) {

            int len = end - start + 1;
            char *substring = (char *) malloc(len + 1);
            strncpy(substring, &strings[0][start], len); // копируем текущую подстроку в выделенную память
            substring[len] = '\0';

            int commonSubstringFound = 1;
            for (int i = 1; i < numStrings; i++) {
                if (strstr(strings[i], substring) == NULL) {
                    commonSubstringFound = 0;
                    break;
                }
            }

            if (commonSubstringFound && len > maxSubstringLen) {
                maxSubstring = substring;
                maxSubstringLen = len;
            } else {
                free(substring);
            }
        }
    }

    return maxSubstring;
}
char* setString() {
    char* str=(char*)malloc(1);
    str[0]='\0';
    int i = 1, c;
    while ((c = getchar()) != EOF && c != '\n') {
        i++;
        str = (char*)realloc(str, i*sizeof(char));
        if (str != NULL) {
            str[i - 2] = c;
            str[i - 1] = '\0';
        }
    }
    return str;
}
int main() {
    system("cls");
    int n;
    printf("Enter amount of strings: ");
    scanf("%d",&n);
    while(getchar()!='\n'|| n<1)
    {
        fflush(stdin);
        printf("Error: Enter amount of strings: ");
        scanf("%d",&n);
    }
    char **strings=(char**)malloc(n*sizeof(char*));
    for(int i=0;i<n;i++) {
        printf("Enter string %d:\n",i+1);
        strings[i]=setString();
        fflush(stdin);
    }

    char *maxSubstring = findMaxCommonSubstring(strings, n);
    if (maxSubstring != NULL) {
        printf("Maximum common substring: %s\n", maxSubstring);
        free(maxSubstring);
    } else {
        printf("Common substring is not founded\n");
    }

    system("pause");
    return 0;
}