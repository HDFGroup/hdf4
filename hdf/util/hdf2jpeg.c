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

#include "hdf.h"
#include "hfile.h"

/* Size of the file buffer to copy through */
#define MAX_FILE_BUF 16384

static uint8 file_buf[MAX_FILE_BUF]; /* size of the buffer to copy through */

static void usage(void);

static void
usage(void)
{
    printf("USAGE: hdf2jpeg <input HDF file> <output JPEG file template>\n");
    printf("        <input HDF file> : the HDF file to extract JPEG images from\n");
    printf("        <output JPEG file template> : template for the name \n");
    printf("                                      of the JPEG file(s) to contain\n");
    printf("                                      the images extracted from the HDF\n");
    printf("                                      file.  The name may contain\n");
    printf("                                      the special character: '#'\n");
    printf("                                      The '#' character will be replaced\n");
    printf("                                      by the reference number of the HDF\n");
    printf("                                      image being extracted.\n");
    printf("                                      The default format is: file.#.jpg\n");
    exit(1);
} /* end usage() */

int
main(int argc, char *argv[])
{
    int32 fid, aid, data_aid;
    int   n, status;
    FILE *jfif_file; /* file ptr for the JFIF file to create */
    dd_t  image_desc;
    intn  jfif_formatted = 0; /* whether the jfif filename has a'#' in it */
    char  jfif_name[128];     /* name of the JPEG file to create */
    char  scratch[128];       /* name of the JPEG file to create */
    char *jfif_char;          /* location of the '#' char */

    /*
     * If a file name has not been supplied print the usage message
     */
    if (argc < 3)
        usage();

    if ((jfif_char = (char *)HDstrchr(argv[2], '#')) == NULL)
        HDstrcpy(jfif_name, argv[2]); /* copy the JPEG file name */
    else {
        if (jfif_char > argv[2]) { /* copy initial portion */
            HDstrncpy(jfif_name, argv[2], (size_t)(jfif_char - argv[2]));
            jfif_name[jfif_char - argv[2]] = '\0'; /* terminate the string */
        }                                          /* end if */
        HDstrcat(jfif_name, "%d");                 /* append formatter */
        HDstrcat(jfif_name, jfif_char + 1);        /* complete the name */
        jfif_formatted = TRUE;
    } /* end else */

    fid = Hopen(argv[1], DFACC_READ, -1);
    if (fid == FAIL) {
        printf("Error opening HDF file: %s\n", argv[1]);
        exit(1);
    } /* end if */

    n   = 0;
    aid = Hstartread(fid, DFTAG_JPEG, DFREF_WILDCARD);
    if (aid != FAIL) {
        do {
            n++; /* increment the number of images found */
            Hinquire(aid, NULL, &image_desc.tag, &image_desc.ref, &image_desc.length, &image_desc.offset,
                     NULL, NULL, NULL);
            if (Hread(aid, image_desc.length, file_buf) != image_desc.length) {
                printf("Error reading %d'th JPEG image from HDF file\n", n);
                exit(1);
            } /* end if */
            if (jfif_formatted == TRUE) {
                sprintf(scratch, jfif_name, image_desc.ref);
                jfif_file = fopen(scratch, "wb");
            } /* end if */
            else {
                jfif_file = fopen(jfif_name, "wb");
            } /* end else */
            if (jfif_file == NULL) {
                printf("error opening JFIF output file\n");
                exit(1);
            } /* end if */
            if (fwrite(file_buf, sizeof(uint8), (size_t)image_desc.length, jfif_file) !=
                (size_t)image_desc.length) {
                printf("Error writing to JPEG file\n");
                exit(1);
            } /* end if */
            data_aid = Hstartread(fid, DFTAG_CI, image_desc.ref);
            Hinquire(data_aid, NULL, &image_desc.tag, &image_desc.ref, &image_desc.length, &image_desc.offset,
                     NULL, NULL, NULL);
            while (image_desc.length > MAX_FILE_BUF) {
                if (Hread(data_aid, MAX_FILE_BUF, file_buf) != (int32)(MAX_FILE_BUF)) {
                    printf("Error reading JPEG image data from HDF file\n");
                    exit(1);
                } /* end if */
                if (fwrite(file_buf, sizeof(uint8), MAX_FILE_BUF, jfif_file) != MAX_FILE_BUF) {
                    printf("Error writing JPEG image data\n");
                    exit(1);
                } /* end if */
                image_desc.length -= MAX_FILE_BUF;
            } /* end while */
            if (image_desc.length > 0) {
                if (Hread(data_aid, image_desc.length, file_buf) != (int32)(image_desc.length)) {
                    printf("Error reading JPEG image data from HDF file\n");
                    exit(1);
                } /* end if */
                if (fwrite(file_buf, sizeof(uint8), (size_t)image_desc.length, jfif_file) !=
                    (size_t)image_desc.length) {
                    printf("Error writing JPEG image data\n");
                    exit(1);
                }                 /* end if */
            }                     /* end if */
            Hendaccess(data_aid); /* let go of the JPEG data AID */
            fclose(jfif_file);    /* close the JFIF file */
            status = Hnextread(aid, DFTAG_JPEG, DFREF_WILDCARD, DF_CURRENT);

        } while (status == SUCCEED);
        Hendaccess(aid);
    } /* end if */

    aid = Hstartread(fid, DFTAG_GREYJPEG, DFREF_WILDCARD);
    if (aid != FAIL) {
        do {
            n++; /* increment the number of images found */
            Hinquire(aid, NULL, &image_desc.tag, &image_desc.ref, &image_desc.length, &image_desc.offset,
                     NULL, NULL, NULL);
            if (Hread(aid, image_desc.length, file_buf) != image_desc.length) {
                printf("Error reading %d'th JPEG image from HDF file\n", n);
                exit(1);
            } /* end if */
            if (jfif_formatted == TRUE) {
                sprintf(scratch, jfif_name, image_desc.ref);
                jfif_file = fopen(scratch, "wb");
            } /* end if */
            else {
                jfif_file = fopen(jfif_name, "wb");
            } /* end else */
            if (jfif_file == NULL) {
                printf("error opening JFIF output file\n");
                exit(1);
            } /* end if */
            if (fwrite(file_buf, sizeof(uint8), (size_t)image_desc.length, jfif_file) !=
                (size_t)image_desc.length) {
                printf("Error writing to JPEG file\n");
                exit(1);
            } /* end if */
            data_aid = Hstartread(fid, DFTAG_CI, image_desc.ref);
            Hinquire(data_aid, NULL, &image_desc.tag, &image_desc.ref, &image_desc.length, &image_desc.offset,
                     NULL, NULL, NULL);
            while (image_desc.length > MAX_FILE_BUF) {
                if (Hread(data_aid, MAX_FILE_BUF, file_buf) != (int32)(MAX_FILE_BUF)) {
                    printf("Error reading JPEG image data from HDF file\n");
                    exit(1);
                } /* end if */
                if (fwrite(file_buf, sizeof(uint8), MAX_FILE_BUF, jfif_file) != (size_t)MAX_FILE_BUF) {
                    printf("Error writing JPEG image data\n");
                    exit(1);
                } /* end if */
                image_desc.length -= MAX_FILE_BUF;
            } /* end while */
            if (image_desc.length > 0) {
                if (Hread(data_aid, image_desc.length, file_buf) != (int32)(image_desc.length)) {
                    printf("Error reading JPEG image data from HDF file\n");
                    exit(1);
                } /* end if */
                if (fwrite(file_buf, sizeof(uint8), (size_t)image_desc.length, jfif_file) !=
                    (size_t)image_desc.length) {
                    printf("Error writing JPEG image data\n");
                    exit(1);
                }                 /* end if */
            }                     /* end if */
            Hendaccess(data_aid); /* let go of the JPEG data AID */
            fclose(jfif_file);    /* close the JFIF file */
            status = Hnextread(aid, DFTAG_GREYJPEG, DFREF_WILDCARD, DF_CURRENT);

        } while (status == SUCCEED);
        Hendaccess(aid);
    } /* end if */

    /* Handle new-style JPEG5 images */
    aid = Hstartread(fid, DFTAG_JPEG5, DFREF_WILDCARD);
    if (aid != FAIL) {
        do {
            Hinquire(aid, NULL, &image_desc.tag, &image_desc.ref, &image_desc.length, &image_desc.offset,
                     NULL, NULL, NULL);
            n++; /* increment the number of images found */
            if (jfif_formatted == TRUE) {
                sprintf(scratch, jfif_name, image_desc.ref);
                jfif_file = fopen(scratch, "wb");
            } /* end if */
            else {
                jfif_file = fopen(jfif_name, "wb");
            } /* end else */
            if (jfif_file == NULL) {
                printf("error opening JFIF output file\n");
                exit(1);
            } /* end if */
            data_aid = Hstartread(fid, DFTAG_CI, image_desc.ref);
            Hinquire(data_aid, NULL, &image_desc.tag, &image_desc.ref, &image_desc.length, &image_desc.offset,
                     NULL, NULL, NULL);
            while (image_desc.length > MAX_FILE_BUF) {
                if (Hread(data_aid, MAX_FILE_BUF, file_buf) != (int32)(MAX_FILE_BUF)) {
                    printf("Error reading JPEG image data from HDF file\n");
                    exit(1);
                } /* end if */
                if (fwrite(file_buf, sizeof(uint8), MAX_FILE_BUF, jfif_file) != MAX_FILE_BUF) {
                    printf("Error writing JPEG image data\n");
                    exit(1);
                } /* end if */
                image_desc.length -= MAX_FILE_BUF;
            } /* end while */
            if (image_desc.length > 0) {
                if (Hread(data_aid, image_desc.length, file_buf) != (int32)(image_desc.length)) {
                    printf("Error reading JPEG image data from HDF file\n");
                    exit(1);
                } /* end if */
                if (fwrite(file_buf, sizeof(uint8), (size_t)image_desc.length, jfif_file) !=
                    (size_t)image_desc.length) {
                    printf("Error writing JPEG image data\n");
                    exit(1);
                }                 /* end if */
            }                     /* end if */
            Hendaccess(data_aid); /* let go of the JPEG data AID */
            fclose(jfif_file);    /* close the JFIF file */
            status = Hnextread(aid, DFTAG_JPEG5, DFREF_WILDCARD, DF_CURRENT);

        } while (status == SUCCEED);
        Hendaccess(aid);
    } /* end if */

    /* Handle new-style GREYJPEG5 images */
    aid = Hstartread(fid, DFTAG_GREYJPEG5, DFREF_WILDCARD);
    if (aid != FAIL) {
        do {
            n++; /* increment the number of images found */
            Hinquire(aid, NULL, &image_desc.tag, &image_desc.ref, &image_desc.length, &image_desc.offset,
                     NULL, NULL, NULL);
            if (jfif_formatted == TRUE) {
                sprintf(scratch, jfif_name, image_desc.ref);
                jfif_file = fopen(scratch, "wb");
            } /* end if */
            else {
                jfif_file = fopen(jfif_name, "wb");
            } /* end else */
            if (jfif_file == NULL) {
                printf("error opening JFIF output file\n");
                exit(1);
            } /* end if */
            data_aid = Hstartread(fid, DFTAG_CI, image_desc.ref);
            Hinquire(data_aid, NULL, &image_desc.tag, &image_desc.ref, &image_desc.length, &image_desc.offset,
                     NULL, NULL, NULL);
            while (image_desc.length > MAX_FILE_BUF) {
                if (Hread(data_aid, MAX_FILE_BUF, file_buf) != (int32)(MAX_FILE_BUF)) {
                    printf("Error reading JPEG image data from HDF file\n");
                    exit(1);
                } /* end if */
                if (fwrite(file_buf, sizeof(uint8), MAX_FILE_BUF, jfif_file) != (size_t)MAX_FILE_BUF) {
                    printf("Error writing JPEG image data\n");
                    exit(1);
                } /* end if */
                image_desc.length -= MAX_FILE_BUF;
            } /* end while */
            if (image_desc.length > 0) {
                if (Hread(data_aid, image_desc.length, file_buf) != (int32)(image_desc.length)) {
                    printf("Error reading JPEG image data from HDF file\n");
                    exit(1);
                } /* end if */
                if (fwrite(file_buf, sizeof(uint8), (size_t)image_desc.length, jfif_file) !=
                    (size_t)image_desc.length) {
                    printf("Error writing JPEG image data\n");
                    exit(1);
                }                 /* end if */
            }                     /* end if */
            Hendaccess(data_aid); /* let go of the JPEG data AID */
            fclose(jfif_file);    /* close the JFIF file */
            status = Hnextread(aid, DFTAG_GREYJPEG5, DFREF_WILDCARD, DF_CURRENT);

        } while (status == SUCCEED);
        Hendaccess(aid);
    } /* end if */

    /* Close the HDF file */
    status = Hclose(fid);

    if (n == 0)
        printf("Error, no JPEG images found in HDF file\n");
    return (0);
}
