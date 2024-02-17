/*
 * Copyright (c) 1987, 1993, 1994
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "h4getopt.h"

#include <stdio.h>
#include <string.h>

int   h4opterr   = 1;    /* if error message should be printed */
int   h4optind   = 1;    /* index into parent argv vector */
int   h4optopt   = 0;    /* character checked for validity */
int   h4optreset = 0;    /* reset getopt */
char *h4optarg   = NULL; /* argument associated with option */

#define BADCH  (int)'?'
#define BADARG (int)':'
#define EMSG   ""

/*
 * getopt --
 *  Parse argc/argv argument vector.
 */
int
h4getopt(int argc, char *const argv[], const char *optstring)
{
    static char *place = EMSG; /* option letter processing */
    char        *oli;          /* option letter list index */

    if (h4optreset || *place == 0) { /* update scanning pointer */
        h4optreset = 0;
        place      = argv[h4optind];
        if (h4optind >= argc || *place++ != '-') {
            /* Argument is absent or is not an option */
            place = EMSG;
            return -1;
        }
        h4optopt = *place++;
        if (h4optopt == '-' && *place == 0) {
            /* "--" => end of options */
            ++h4optind;
            place = EMSG;
            return -1;
        }
        if (h4optopt == 0) {
            /* Solitary '-', treat as a '-' option
               if the program (eg su) is looking for it. */
            place = EMSG;
            if (strchr(optstring, '-') == NULL)
                return -1;
            h4optopt = '-';
        }
    }
    else
        h4optopt = *place++;

    /* See if option letter is one the caller wanted... */
    if (h4optopt == ':' || (oli = strchr(optstring, h4optopt)) == NULL) {
        if (*place == 0)
            ++h4optind;
        if (h4opterr && *optstring != ':')
            fprintf(stderr, "unknown option -- %c\n", h4optopt);
        return BADCH;
    }

    /* Does this option need an argument? */
    if (oli[1] != ':') {
        /* don't need argument */
        h4optarg = NULL;
        if (*place == 0)
            ++h4optind;
    }
    else {
        /* Option-argument is either the rest of this argument or the
           entire next argument. */
        if (*place)
            h4optarg = place;
        else if (argc > ++h4optind)
            h4optarg = argv[h4optind];
        else {
            /* option-argument absent */
            place = EMSG;
            if (*optstring == ':')
                return BADARG;
            if (h4opterr)
                fprintf(stderr, "option requires an argument -- %c\n", h4optopt);
            return BADCH;
        }
        place = EMSG;
        ++h4optind;
    }
    return h4optopt; /* return option letter */
}
