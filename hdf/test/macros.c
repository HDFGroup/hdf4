/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdint.h>

#include "testhdf.h"

extern int Verbosity;

void
test_macros(void)
{

    uint8_t *p;
    uint16   new16u, old16u, str16u;
    int16    new16, old16, str16;
    uint32   new32u, old32u, str32u;
    int32    new32, old32, str32;
    int      errors1, errors2, errors3, errors4;
    int      errors = 0;

    uint16 data1[10] = {0,         1, 2, 3, 4, UINT16_MAX - 4, UINT16_MAX - 3, UINT16_MAX - 2, UINT16_MAX - 1,
                        UINT16_MAX};

    int16 data2[20] = {-INT16_MAX - 1,
                       -INT16_MAX,
                       -INT16_MAX + 1,
                       -INT16_MAX + 2,
                       -INT16_MAX + 3,
                       -INT16_MAX + 4,
                       -4,
                       -3,
                       -2,
                       -1,
                       0,
                       1,
                       2,
                       3,
                       4,
                       INT16_MAX - 4,
                       INT16_MAX - 3,
                       INT16_MAX - 2,
                       INT16_MAX - 1,
                       INT16_MAX};

    uint32 data3[10] = {0,         1, 2, 3, 4, UINT32_MAX - 4, UINT32_MAX - 3, UINT32_MAX - 2, UINT32_MAX - 1,
                        UINT32_MAX};

    int32 data4[20] = {-INT32_MAX - 1,
                       -INT32_MAX,
                       -INT32_MAX + 1,
                       -INT32_MAX + 2,
                       -INT32_MAX + 3,
                       -INT32_MAX + 4,
                       -4,
                       -3,
                       -2,
                       -1,
                       0,
                       1,
                       2,
                       3,
                       4,
                       INT32_MAX - 4,
                       INT32_MAX - 3,
                       INT32_MAX - 2,
                       INT32_MAX - 1,
                       INT32_MAX};

    if (Verbosity > 5)
        printf("\n");
    errors1 = 0;
    for (size_t j = 0; j < 10; j++) {
        old16u = data1[j];

        p = (uint8_t *)&str16u;
        UINT16ENCODE(p, old16u);
        p = (uint8_t *)&str16u;
        UINT16DECODE(p, new16u);

        if (old16u != new16u) {
            if (Verbosity > 8) {
                printf("old16u = %" PRIu16 ", %" PRIx16 "\n", old16u, old16u);
                printf("str: ");
                p = (uint8_t *)&str16u;
                for (size_t i = 0; i < sizeof(uint16); i++) {
                    printf("%x ", 0xff & *p++);
                }
                printf("\n");
                printf("new16u = %" PRIu16 ", %" PRIx16 "\n", new16u, new16u);
                printf("\n");
            }
            errors1++;
        }
    }
    if (Verbosity > 5) {
        if (errors1 == 0) {
            printf("UNSIGNED INTEGER16: SUCCESSFUL\n");
        }
        else {
            printf("UNSIGNED INTEGER16: %d ERRORS\n", errors1);
        }
    }

    if (Verbosity > 5)
        printf("\n");
    errors2 = 0;
    for (size_t j = 0; j < 20; j++) {
        old16 = data2[j];

        p = (uint8_t *)&str16;
        INT16ENCODE(p, old16);
        p = (uint8_t *)&str16;
        INT16DECODE(p, new16);

        if (old16 != new16) {
            if (Verbosity > 8) {
                printf("old16 = %" PRId16 ", %" PRIx16 "\n", old16, old16);
                printf("str: ");
                p = (uint8_t *)&str16;
                for (size_t i = 0; i < sizeof(int16); i++) {
                    printf("%x ", 0xff & *p++);
                }
                printf("\n");
                printf("new16 = %" PRId16 ", %" PRIx16 "\n", new16, new16);
                printf("\n");
            }
            errors2++;
        }
    }
    if (Verbosity > 5) {
        if (errors2 == 0) {
            printf("SIGNED INTEGER16: SUCCESSFUL\n");
        }
        else {
            printf("SIGNED INTEGER16: %d ERRORS\n", errors2);
        }
    }

    if (Verbosity > 5)
        printf("\n");
    errors3 = 0;
    for (size_t j = 0; j < 10; j++) {
        old32u = data3[j];

        p = (uint8_t *)&str32u;
        UINT32ENCODE(p, old32u);
        p = (uint8_t *)&str32u;
        UINT32DECODE(p, new32u);

        if (old32u != new32u) {
            if (Verbosity > 8) {
                printf("old32u = %" PRIu32 ", %" PRIx32 "\n", old32u, old32u);
                printf("str: ");
                p = (uint8_t *)&str32u;
                for (size_t i = 0; i < sizeof(uint32); i++) {
                    printf("%x ", 0xff & *p++);
                }
                printf("\n");
                printf("new32u = %" PRIu32 ", %" PRIx32 "\n", new32u, new32u);
                printf("\n");
            }
            errors3++;
        }
    }
    if (Verbosity > 5) {
        if (errors3 == 0) {
            printf("UNSIGNED INTEGER32: SUCCESSFUL\n");
        }
        else {
            printf("UNSIGNED INTEGER32: %d ERRORS\n", errors3);
        }
    }

    if (Verbosity > 5)
        printf("\n");
    errors4 = 0;
    for (size_t j = 0; j < 20; j++) {
        old32 = data4[j];

        p = (uint8_t *)&str32;
        INT32ENCODE(p, old32);
        p = (uint8_t *)&str32;
        INT32DECODE(p, new32);

        if (old32 != new32) {
            if (Verbosity > 8) {
                printf("old32 = %" PRId32 ", %" PRIx32 "\n", old32, old32);
                printf("str: ");
                p = (uint8_t *)&str32;
                for (size_t i = 0; i < sizeof(int32); i++) {
                    printf("%x ", 0xff & *p++);
                }
                printf("\n");
                printf("new32 = %" PRId32 ", %" PRIx32 "\n", new32, new32);
                printf("\n");
            }
            errors4++;
        }
    }
    if (Verbosity > 5) {
        if (errors4 == 0) {
            printf("SIGNED INTEGER32: SUCCESSFUL\n");
        }
        else {
            printf("SIGNED INTEGER32: %d ERRORS\n", errors4);
        }
    }

    errors = errors1 + errors2 + errors3 + errors4;
    if (errors > 0) {
        printf("            %d ERRORS were detected during (macros) Testing\n", errors);
        num_errs += errors;
    }

} /* end test_macros() */
