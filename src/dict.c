#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#define IS_WORD_BIT     256
#define END_OF_LIST_BIT 512
#define CHAR_MASK       255
#define ADDRESS_SHIFT   10

static int *dawg = NULL;
static int dawg_len;

int load_dawg(char *filename)
{
    unsigned char entry[4];
    FILE *infile;
    struct stat statbuf;

    if (stat(filename, &statbuf)) {
        perror("dict(stat)");
        return -1;
    }

    if (!(infile = fopen(filename, "r")))
    {
        perror("dict(fopen)");
        return -1;
    }

    if (dawg != NULL) {
        free(dawg);
        dawg = NULL;
        dawg_len = 0;
    }

    printf("dawg file size = %ld\n", statbuf.st_size);
    dawg = (int *) malloc(statbuf.st_size);

    while (fread(&entry, 1, 4, infile) == 4)
    {
        dawg[dawg_len++] = (entry[0] << 24) + (entry[1] << 16) +
            (entry[2] << 8) + entry[3];
    }

    fclose(infile);
}

int is_word(int dawg_ptr)
{
    return (dawg[dawg_ptr] & IS_WORD_BIT) != 0;
}

int is_eol(int dawg_ptr)
{
    return (dawg[dawg_ptr] & END_OF_LIST_BIT) != 0;
}

int letter(int dawg_ptr)
{
    return dawg[dawg_ptr] & CHAR_MASK;
}

int next(int dawg_ptr)
{
    return dawg[dawg_ptr] >> ADDRESS_SHIFT;
}

int dawg_start()
{
    return dawg_len - 26;
}

int match_word(char *word, int offset, int dawg_ptr, int longest_match)
{
    char word_ch, dawg_ch;

    word_ch = word[offset];

    // word_ch should be lowercase
    if ((word_ch >= 'A') && (word_ch <= 'Z'))
    {
        word_ch = word_ch - 'A' + 'a';
    }

    for (;;)
    {
        // Get the letter that the current dawgptr contains
        dawg_ch = letter(dawg_ptr);

        // Does it match the current word?
        if (dawg_ch == word_ch)
        {
            // If this position in the DAWG indicates a word, it is the
            // longest word
            if (is_word(dawg_ptr))
            {
                longest_match = offset+1;
            }

            // If this is the end of the word, it's the longest match
            if (!word[offset+1])
            {
                return longest_match;
            }
            else
            {
                // Otherwise, keep matching
                return match_word(word, offset+1, next(dawg_ptr), longest_match);
            }
        }
        // If this is the end of the list in the dawg, there are no more
        // letters that could come after the current prefix,
        // so return the current longest match
        if (is_eol(dawg_ptr)) return longest_match;

        // Try the next letter in the list
        dawg_ptr++;
    }
}

int match(char *word)
{
    return match_word(word, 0, dawg_start(), 0) == strlen(word);
}


int longest_match(char *word)
{
    return match_word(word, 0, dawg_start(), 0);
}
