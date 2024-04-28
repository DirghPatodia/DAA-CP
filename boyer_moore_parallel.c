#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define NO_OF_CHARS 256

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void badCharHeuristic(char *pattern, int patternLength, int badchar[NO_OF_CHARS])
{
    for (int i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    for (int i = 0; i < patternLength; i++)
        badchar[(int)pattern[i]] = i;
}

void parallelSearch(char *text, char *pattern, int textLength, int patternLength, int badchar[NO_OF_CHARS], int line_index)
{
    int s = 0;
    while (s <= (textLength - patternLength))
    {
        int j;
        for (j = 0; j < patternLength; j++)
        {
            if (pattern[j] != text[s + j])
                break;
        }

        if (j == patternLength)
        {
#pragma omp critical
            printf("Pattern found in string %d at index %d\n", line_index, s);
        }
        s++;
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
    FILE *fp = fopen("gene.txt", "r");

    if (fp == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    char *text = malloc(fileSize + 1);

    if (text == NULL)
    {
        perror("Memory allocation error");
        fclose(fp);
        return EXIT_FAILURE;
    }

    fread(text, 1, fileSize, fp);
    text[fileSize] = '\0';
    fclose(fp);

    char pattern[100]; // Assuming maximum pattern length is 100 characters
    printf("Enter the pattern to search: ");
    scanf("%s", pattern);

    int patternLength = strlen(pattern);
    int badchar[NO_OF_CHARS];
    badCharHeuristic(pattern, patternLength, badchar);

    char **lines;
    int num_lines;
    read_strings_from_file("gene.txt", &lines, &num_lines);

#pragma omp parallel for
    for (int i = 0; i < num_lines; i++)
    {
        parallelSearch(lines[i], pattern, strlen(lines[i]), patternLength, badchar, i);
        free(lines[i]); // Free allocated memory for individual lines
    }

    free(text);
    free(lines); // Free allocated memory for the array of lines

    return 0;
}
