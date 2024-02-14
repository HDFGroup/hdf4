#include <stdio.h>
#include <stdlib.h>

#include "hdf.h"

#define FILE_NAME "General_HDFobjects.hdf"
#define VG_NAME   "AN Vgroup"

int
main()
{
    /************************* Variable declaration **************************/

    int32    file_id, an_id;
    int32    n_annots;                  /* number of annotations */
    int32   *ann_list = NULL;           /* list of annotation identifiers */
    int32    vgroup_ref;                /* reference number of the vgroup */
    int32    index;                     /* index of an annotation in the annotation list */
    ann_type annot_type = AN_DATA_DESC; /* annotation to be obtained*/
    uint16   ann_tag, ann_ref,          /* tag/ref number of an annotation */
        vgroup_tag = DFTAG_VG;          /* tag of the vgroup */

    /********************** End of variable declaration **********************/

    /*
     * Open the HDF file.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_READ, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the V interface.
     */
    if (Vstart(file_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Get the vgroup named VG_NAME.
     */
    if ((vgroup_ref = Vfind(file_id, VG_NAME)) == FAIL)
        printf("*** ERROR from Vfind\n");

    /*
     * Initialize the AN interface and obtain an interface id.
     */
    if ((an_id = ANstart(file_id)) == FAIL)
        printf("*** ERROR from ANstart\n");

    /*
     * Get the number of object descriptions.  Note that, since ANnumann takes
     * the tag and reference number as being of type unit16, vgroup_ref must be
     * safely cast to uint16 by checking for FAIL value first.
     */
    if (vgroup_ref != FAIL) {
        n_annots = ANnumann(an_id, annot_type, vgroup_tag, (uint16)vgroup_ref);

        /*
         * Allocate space to hold the annotation identifiers.
         */
        ann_list = malloc(n_annots * sizeof(int32));

        /*
         * Get the list of identifiers of the annotations attached to the
         * vgroup and of type annot_type.
         */
        n_annots = ANannlist(an_id, annot_type, vgroup_tag, (uint16)vgroup_ref, ann_list);

        /*
         * Get each annotation identifier from the list then display the
         * tag/ref number pair of the corresponding annotation.
         */
        printf("List of annotations of type AN_DATA_DESC:\n");
        for (index = 0; index < n_annots; index++) {
            /*
             * Get and display the ref number of the annotation from
             * its identifier.
             */
            if (ANid2tagref(ann_list[index], &ann_tag, &ann_ref) == FAIL)
                printf("*** ERROR from ANid2tagref\n");
            printf("Annotation index %d: tag = %s\nreference number= %d\n", index,
                   ann_tag == DFTAG_DIA ? "DFTAG_DIA (data description)" : "Incorrect", ann_ref);
        } /* for */
    }     /* for */

    /*
     * Get and display an annotation type from an annotation tag.
     */
    annot_type = ANtag2atype(DFTAG_FID);
    printf("\nAnnotation type of DFTAG_FID (file label) is %s\n",
           annot_type == AN_FILE_LABEL ? "AN_FILE_LABEL" : "Incorrect");

    /*
     * Get and display an annotation tag from an annotation type.
     */
    ann_tag = ANatype2tag(AN_DATA_LABEL);
    printf("\nAnnotation tag of AN_DATA_LABEL is %s\n",
           ann_tag == DFTAG_DIL ? "DFTAG_DIL (data label)" : "Incorrect");

    /*
     * Terminate access to the AN interface and close the HDF file.
     */
    if (ANend(an_id) == FAIL)
        printf("*** ERROR from ANend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    /*
     * Free the space allocated for the annotation identifier list.
     */
    free(ann_list);
    return 0;
}
