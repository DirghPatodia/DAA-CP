#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define d 256 // Number of characters in the input alphabet
#define q 101 // A prime number for the hash function

// Function to implement the Rabin-Karp algorithm
void searchRabinKarp(char *pattern, char *text, int text_index)
{
    int M = strlen(pattern);
    int N = strlen(text);
    int i, j;
    int p = 0; // Hash value for pattern
    int t = 0; // Hash value for current window of text
    int h = 1;

    // Calculate h, the hash multiplier
    for (i = 0; i < M - 1; i++)
    {
        h = (h * d) % q;
    }

    // Calculate the hash value of the pattern and the first window of text
    for (i = 0; i < M; i++)
    {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    // Slide the pattern over the text one character at a time
    for (i = 0; i <= N - M; i++)
    {
        // Check the hash values of the current window of text and the pattern
        if (p == t)
        {
            // If the hash values match, check each character in the pattern
            for (j = 0; j < M; j++)
            {
                if (text[i + j] != pattern[j])
                    break;
            }

            // If pattern matches the window, print the index
            if (j == M)
            {
                printf("Pattern found in string %d at index %d\n", text_index, i);
            }
        }

        // Calculate the hash value for the next window of text
        if (i < N - M)
        {
            t = (d * (t - text[i] * h) + text[i + M]) % q;

            // Ensure the hash value is non-negative
            if (t < 0)
            {
                t = (t + q);
            }
        }
    }
}

int main()
{
    FILE *file;
    char *filename = "gene.txt"; // Replace with the filename containing text

    // Open the file
    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Ask the user for the pattern
    char pattern[100]; // Assuming maximum pattern length is 100 characters
    printf("Enter the pattern to search: ");
    if (scanf("%s", pattern) != 1)
    {
        printf("Error reading pattern from input.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read the text from the file
    char *text = NULL;
    size_t len = 0;
    int text_index = 0;

    while (getline(&text, &len, file) != -1)
    {
        text[strcspn(text, "\n")] = '\0'; // Remove newline character
        searchRabinKarp(pattern, text, text_index++);
    }

    free(text);
    fclose(file);

    return 0;
}
