#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void search(const char *text, const char *pat, int text_index)
{
    int n = strlen(text);
    int m = strlen(pat);
    for (int i = 0; i <= n - m; i++)
    {
        if (strncmp(pat, &text[i], m) == 0)
        {
            printf("Pattern found in string %d at index %d\n", text_index, i);
        }
    }
}

int main()
{
    char pat[100];
    char **strings = NULL;
    int num_strings = 0;
    size_t size = 0;

    // Open the file
    FILE *fp = fopen("gene.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read strings from file
    char *line = NULL;
    while (getline(&line, &size, fp) != -1)
    {
        // Allocate memory for the new string
        strings = (char **)realloc(strings, sizeof(char *) * (num_strings + 1));
        strings[num_strings] = strdup(line); // Duplicate the line
        num_strings++;
    }
    free(line); // Free the memory allocated by getline

    // Close the file
    fclose(fp);

    // Input pattern
    printf("Enter the pattern to search: ");
    scanf("%s", pat);

    // Search for the pattern in each string
    for (int i = 0; i < num_strings; i++)
    {
        search(strings[i], pat, i);
        free(strings[i]); // Free allocated memory for individual strings
    }
    free(strings); // Free allocated memory for the array of strings

 return 0;
}