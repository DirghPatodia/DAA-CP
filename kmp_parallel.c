#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void computeLPSArray(char *pattern, int M, int *lps)
{
    int len = 0;
    int i = 1;

    lps[0] = 0;

    while (i < M)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void parallelKMPSearchLine(char *pattern, char *text, int line_index)
{
    int M = strlen(pattern);
    int N = strlen(text);

    int *lps = (int *)malloc(sizeof(int) * M);
    if (lps == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    computeLPSArray(pattern, M, lps);

    int i, j;
    int pattern_found = 0;

    for (i = 0; i <= N - M; i++)
    {
        if (pattern_found)
            break;

        j = 0;

        while (j < M && pattern[j] == text[i + j])
        {
            j++;
        }

        if (j == M)
        {
            printf("Pattern found in string %d at index %d\n", line_index, i);
            pattern_found = 1;
        }
    }

    free(lps);
}

int main()
{
    FILE *file;
    char *filename = "gene.txt";
    char pattern[100]; // Assuming maximum pattern length is 100 characters
    printf("Enter the pattern to search: ");
    scanf("%s", pattern);

    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char *line = NULL;
    size_t len = 0;
    int line_index = 0;

    while (getline(&line, &len, file) != -1)
    {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        parallelKMPSearchLine(pattern, line, line_index++);
    }

    free(line);
    fclose(file);

    return 0;
}
