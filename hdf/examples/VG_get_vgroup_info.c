#include "hdf.h"

#define  FILE_NAME   "General_Vgroups.hdf"

int main( )
{
   /************************* Variable declaration **************************/

   int32  file_id, vgroup_id;
   int32  lone_vg_number,      /* current lone vgroup number */
          num_of_lones = 0;    /* number of lone vgroups */
   int32 *ref_array = NULL;    /* buffer to hold the ref numbers of lone vgroups   */
   char  *vgroup_name, *vgroup_class;
   uint16 name_len;

   /********************** End of variable declaration **********************/

   /*
    * Open the HDF file for reading.
    */
   file_id = Hopen (FILE_NAME, DFACC_READ, 0);

   /*
    * Initialize the V interface.
    */
   Vstart (file_id);

   /*
    * Get and print the names and class names of all the lone vgroups.
    * First, call Vlone with num_of_lones set to 0 to get the number of
    * lone vgroups in the file, but not to get their reference numbers.
    */
   num_of_lones = Vlone (file_id, NULL, num_of_lones );

   /*
    * Then, if there are any lone vgroups,
    */
   if (num_of_lones > 0)
   {
       /*
        * use the num_of_lones returned to allocate sufficient space for the
        * buffer ref_array to hold the reference numbers of all lone vgroups,
        */
       ref_array = (int32 *) malloc(sizeof(int32) * num_of_lones);

       /*
        * and call Vlone again to retrieve the reference numbers into
        * the buffer ref_array.
        */
       num_of_lones = Vlone (file_id, ref_array, num_of_lones);

       /*
        * Display the name and class of each lone vgroup.
        */
       fprintf(stderr, "Lone vgroups in this file are:\n");
       for (lone_vg_number = 0; lone_vg_number < num_of_lones; lone_vg_number++)
       {
           /*
            * Attach to the current vgroup then get and display its
            * name and class. Note: the current vgroup must be detached before
            * moving to the next.
            */
           vgroup_id = Vattach (file_id, ref_array[lone_vg_number], "r");
           Vgetnamelen(vgroup_id, &name_len);
           vgroup_name = (char *) HDmalloc(sizeof(char *) * (name_len+1));
           if (vgroup_name == NULL)
           {
               fprintf(stderr, "Not enough memory for vgroup_name!\n");
               exit(1);
           }
           Vgetname (vgroup_id, vgroup_name);

           Vgetclassnamelen(vgroup_id, &name_len);
           vgroup_class = (char *) HDmalloc(sizeof(char *) * (name_len+1));
           if (vgroup_class == NULL)
           {
               fprintf(stderr, "Not enough memory for vgroup_class!\n");
               exit(1);
           }
           Vgetclass (vgroup_id, vgroup_class);
           fprintf(stderr, "   Vgroup name %s and class %s\n", vgroup_name, vgroup_class);
           Vdetach (vgroup_id);
           if (vgroup_name != NULL) HDfree(vgroup_name);
           if (vgroup_class != NULL) HDfree(vgroup_class);
       } /* for */
   } /* if */

   /*
   * Terminate access to the V interface and close the file.
   */
   Vend (file_id);
   Hclose (file_id);

   /*
   * Free the space allocated by this program.
   */
   free (ref_array);
   return 0;
}
