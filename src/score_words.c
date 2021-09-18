#include <memory.h>
#include <stdlib.h>

#include "dict.h"

static char loaded_dawg;

double score_segment(char *buffer, int buffer_len, int pos, char *cached_score, double *score_cache);

double *score_cache = NULL;
char *cached_score = NULL;
int score_cache_size = 0;

double score(char *buffer, int buffer_len)
{
    long score;
    int dawg_ptr, pos, word_len, word_start;
    char word[15], *dict;

    if (!loaded_dawg)
    {
        dict=getenv("SCORE_DICT");
        if (!dict) dict = "dict/enable1.daw";

        if (load_dawg(dict) < 0) {
            exit(1);
        }
        loaded_dawg = 1;
    }

    if (score_cache_size < buffer_len) {
        if (score_cache != NULL) {
            free(score_cache);
            free(cached_score);
            score_cache_size = 0;
        }
        // Allocate a cache for score values
        score_cache = (double *) malloc(buffer_len * sizeof(double));
        cached_score = (char *) malloc(buffer_len);
        score_cache_size = buffer_len;
    }

    // Set all the cached flags to 0
    for (int i=0; i < buffer_len; i++) cached_score[i] = 0;

    // Do scores from the end of the buffer and working backwards to avoid
    // deep recursion
    for (int i=buffer_len-1; i >= 0; i--) {
        score_segment(buffer, buffer_len, i, cached_score, score_cache);
    }

    return score_segment(buffer, buffer_len, 0, cached_score, score_cache);
}

double score_segment(char *buffer, int buffer_len, int pos, char *cached_score, double *score_cache)
{
    long score, score2;
    int i, end, longest;
    char word[16];

    if (pos >= buffer_len) return 0;
    if (cached_score[pos]) return score_cache[pos];

    end = 15;
    if (pos + end > buffer_len) end = buffer_len - pos;

    memcpy(word, &buffer[pos], end);
    word[end] = 0;

    score = 0;
    score2 = 0;

    for (i=0; i < end; i++) 
    {
        if (word[i] < 26) word[i] = 'a' + word[i];
    }

    // Find the longest word that can start from this position
    longest = longest_match(word);

    if (longest > 0)
    {
        // Sum the squares of the lengths of the longest words
        score = longest * longest;

        // Score the rest of the buffer starting from end of this word
        score = score + score_segment(buffer, buffer_len, pos + longest, cached_score, score_cache);
    }
    else
    {
        // Skip the current character and score the rest of the buffer
        score = score_segment(buffer, buffer_len, pos + 1, cached_score, score_cache);
    }

    cached_score[pos] = 1;
    score_cache[pos] = score;
    return score;
}
