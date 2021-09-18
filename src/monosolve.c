#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#include "score_words.h"

#define MAX_BUFFER_LEN 30000

char buffer[MAX_BUFFER_LEN];
char temp_buffer[MAX_BUFFER_LEN];
int buf_len = 0;

double score(char *buffer, int buffer_len);

int main(int argc, char *argv[])
{
    FILE *infile;
    int ch, i, j, k, temp, change;
    char best_map[26], curr_map[26];
    double all_best_score, best_score, curr_score, temp_best;
    int temp_exchange, num_no_change;
    long now;

    infile = stdin;

    if (argc > 1)
    {
            // Open the input file
        if ((infile = fopen(argv[1], "r")) == NULL)
        {
            fprintf(stderr, "Can't open %s\n", argv[1]);
            perror("fopen");
            exit(1);
        }
    }

    while (((ch = fgetc(infile)) != EOF) && (buf_len < sizeof(buffer)))
    {
        if ((ch >= 'A') && (ch <= 'Z'))
        {
            ch -= 'A';
        }
        else if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a';
        }
        else
        {
            // Skip non-letters
            continue;
        }

        buffer[buf_len++] = ch;
    }

    time(&now);
    srand(now);

    // No all-time best score
    all_best_score = -99999.0;

    // Initialize number of times there was no change in the score
    num_no_change = 0;

    for (;;)
    {
        best_score = -99999.0;

        // Initialize the current key
        for (i=0; i < 26; i++) curr_map[i] = i;

        // Randomly scramble the current key by exchanging each
        // item with a random other
        for (i=0; i < 26; i++)
        {
            j = rand() % 26;
            temp = curr_map[i];
            curr_map[i] = curr_map[j];
            curr_map[j] = temp;
        }

        change = 1;

        // While the best score changed from the previous run
        while (change)
        {
            change = 0;

            // For each letter in the key...
            for (i=0; i < 26; i++)
            {
                temp_best = -99999.0;

                // Try exchanging it with each other letter in the key
                for (j=0; j < 26; j++)
                {
                    if (j ==i) continue;

                    temp = curr_map[i];
                    curr_map[i] = curr_map[j];
                    curr_map[j] = temp;

                    // Decrypt the ciphertext with the current key
                    for (k=0; k < buf_len; k++)
                    {
                        temp_buffer[k] = curr_map[buffer[k]];
                    }

                    // Compute a score from the decrypted text
                    curr_score = score(temp_buffer, buf_len);

                    // If this score is better than the best so far
                    // then position j is the best position to swap i with
                    if (curr_score > temp_best)
                    {
                        temp_best = curr_score;
                        temp_exchange = j;
                    }

                    // Swap i and j back to their original places
                    temp = curr_map[i];
                    curr_map[i] = curr_map[j];
                    curr_map[j] = temp;
                }

                // If the best score from all possible exchanges improves
                // over the previous best, commit the exchange as the
                // best key so far
                if (temp_best > best_score)
                {
                    temp = curr_map[i];
                    curr_map[i] = curr_map[temp_exchange];
                    curr_map[temp_exchange] = temp;

                    best_score = temp_best;

                    memcpy(best_map, curr_map, sizeof(curr_map));
                    change = 1;
                }
            }
        }

        // If this score is better than the previous best, print the
        // key and decrypted text
        if ((best_score > all_best_score) || (num_no_change > 100))
        {
            for (i=0; i < 26; i++)
            {
                putchar('a'+best_map[i]);
            }
            putchar('\n');

            for (i=0; i < 26; i++)
            {
                putchar('A'+i);
            }
            putchar('\n');
            putchar('\n');

            for (i=0; i < buf_len; i++)
            {
                putchar('a'+best_map[buffer[i]]);
            }
            putchar('\n');

            if (best_score <= all_best_score)
            {
                all_best_score = 0;
            }
            else
            {
                all_best_score = best_score;
            }
            if (num_no_change >= 100) {
                exit(0);
            }
        }
        else
        {
            num_no_change++;
        }
    }
}
