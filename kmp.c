#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to compute the LPS (Longest Proper Prefix which is also Suffix) array
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

// Function to perform pattern searching using KMP algorithm for a single line
void KMPSearchLine(char *pattern, char *text, int line_index)
{
    int M = strlen(pattern);
    int N = strlen(text);

    // Create lps[] that will hold the Longest Proper Prefix values for pattern
    int *lps = (int *)malloc(sizeof(int) * M);
    if (lps == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    // Preprocess the pattern
    computeLPSArray(pattern, M, lps);

    int i = 0; // Index for text[]
    int j = 0; // Index for pattern[]

    while (i < N)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == M)
        {
            printf("Pattern found in string %d at index %d\n", line_index, i - j);
            j = lps[j - 1];
        }
        else if (i < N && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
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
    scanf("%s", pattern); // Replace with the pattern you want to search

    // Open the file
    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read each line from the file and perform pattern search
    char *line = NULL;
    size_t len = 0;
    int line_index = 0;
    while (getline(&line, &len, file) != -1)
    {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        KMPSearchLine(pattern, line, line_index++);
    }

    free(line); // Free allocated memory for line buffer

    fclose(file);

    return 0;
}
