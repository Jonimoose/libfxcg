/*
 * (c) Copyright 2012-2016 by Einar Saukas. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name of its author may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !TARGET_PRIZM

#include "zx7.h"

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>

static int elias_gamma_bits(int value) {
    int bits;

    bits = 1;
    while (value > 1) {
        bits += 2;
        value >>= 1;
    }
    return bits;
}

static int count_bits(int offset, int len) {
    return 1 + (offset > 128 ? 12 : 8) + elias_gamma_bits(len-1);
}

Optimal* optimize(const unsigned char *input_data, unsigned int input_size, unsigned long skip) {
    unsigned int *min;
    unsigned int *max;
    unsigned int *matches;
    unsigned int *match_slots;
    Optimal *optimal;
    unsigned int *match;
    int match_index;
    int offset;
    unsigned int len;
    unsigned int best_len;
    unsigned int bits;
    unsigned int i;

    /* allocate all data structures at once */
    min = (unsigned int *)calloc(MAX_OFFSET+1, sizeof(unsigned int));
    max = (unsigned int *)calloc(MAX_OFFSET+1, sizeof(unsigned int));
    matches = (unsigned int *)calloc(256*256, sizeof(unsigned int));
    match_slots = (unsigned int *)calloc(input_size, sizeof(unsigned int));
    optimal = (Optimal *)calloc(input_size, sizeof(Optimal));

    if (!min || !max || !matches || !match_slots || !optimal) {
		return 0;
    }

    /* index skipped bytes */
    for (i = 1; i <= skip; i++) {
        match_index = input_data[i-1] << 8 | input_data[i];
        match_slots[i] = matches[match_index];
        matches[match_index] = i;
    }

    /* first byte is always literal */
    optimal[skip].bits = 8;

    /* process remaining bytes */
    for (; i < input_size; i++) {

        optimal[i].bits = optimal[i-1].bits + 9;
        match_index = input_data[i-1] << 8 | input_data[i];
        best_len = 1;
        for (match = &matches[match_index]; *match != 0 && best_len < MAX_LEN; match = &match_slots[*match]) {
            offset = i - *match;
            if (offset > MAX_OFFSET) {
                *match = 0;
                break;
            }

            for (len = 2; len <= MAX_LEN && i >= skip+len; len++) {
                if (len > best_len) {
                    best_len = len;
                    bits = optimal[i-len].bits + count_bits(offset, len);
                    if (optimal[i].bits > bits) {
                        optimal[i].bits = bits;
                        optimal[i].offset = offset;
                        optimal[i].len = len;
                    }
                } else if (max[offset] != 0 && i+1 == max[offset]+len) {
                    len = i-min[offset];
                    if (len > best_len) {
                        len = best_len;
                    }
                }
                if (i < offset+len || input_data[i-len] != input_data[i-len-offset]) {
                    break;
                }
            }
            min[offset] = i+1-len;
            max[offset] = i;
        }
        match_slots[i] = matches[match_index];
        matches[match_index] = i;
    }

    free(match_slots);
	free(min);
	free(max);
	free(matches);

    return optimal;
}


#ifdef __cplusplus
}
#endif

#endif