#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_OF_CHARS 256

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void badCharHeuristic(char *pattern, int patternLength, int badchar[NO_OF_CHARS])
{
    int i;

    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    for (i = 0; i < patternLength; i++)
        badchar[(int)pattern[i]] = i;
}

void search(char *text, char *pattern, int text_index)
{
    int patternLength = strlen(pattern);
    int textLength = strlen(text);

    int badchar[NO_OF_CHARS];

    badCharHeuristic(pattern, patternLength, badchar);

    int s = 0;
    while (s <= (textLength - patternLength))
    {
        int j = patternLength - 1;

        while (j >= 0 && pattern[j] == text[s + j])
            j--;

        if (j < 0)
        {
            printf("Pattern foundin string %d at index %d \n", text_index ,s);
            s += (s + patternLength < textLength) ? patternLength - badchar[text[s + patternLength]] : 1;
        }
        else
        {
            s += max(1, j - badchar[text[s + j]]);
        }
    }
}

void read_strings_from_file(const char *file_path, char ***strings, int *num_strings)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int size = 1024; // Initial size of each string buffer
    *num_strings = 0;
    *strings = (char **)malloc(sizeof(char *) * size);

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (*num_strings >= size)
        {
            size *= 2;
            *strings = (char **)realloc(*strings, sizeof(char *) * size);
        }
        (*strings)[*num_strings] = strdup(line);
        (*num_strings)++;
    }

    fclose(file);
}

int main()
{
    char file_path[] = "gene.txt";
    char *pattern = NULL;
    char **strings;
    int num_strings;

    printf("Enter the pattern to search: ");
    size_t len = 0;
    getline(&pattern, &len, stdin);
    pattern[strcspn(pattern, "\n")] = 0; // Remove newline character from the pattern

    read_strings_from_file(file_path, &strings, &num_strings);

    for (int i = 0; i < num_strings; i++)
    {
        search(strings[i], pattern, i);
        free(strings[i]); // Free allocated memory for individual strings
    }

    free(pattern); // Free allocated memory for the pattern
    free(strings); // Free allocated memory for the array of strings

    return 0;
}
