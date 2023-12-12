#include "hdf.h"

#define  FILE_NAME   "General_HDFobjects.hdf"

int main() {
	/************************* Variable declaration **************************/

	intn status_n; /* returned status for functions returning an intn  */
	int32 status_32; /* returned status for functions returning an int32 */
	int32 file_id; /* HDF file identifier */
	int32 an_id; /* AN interface identifier */
	int32 ann_id; /* an annotation identifier */
	int32 index; /* position of an annotation in all of the same type*/
	int32 ann_length; /* length of the text in an annotation */
	int32 n_file_labels;
	int32 n_file_descs;
	int32 n_data_labels;
	int32 n_data_descs;
	char *ann_buf; /* buffer to hold the read annotation */
	int ret_val = FAIL;

	/********************** End of variable declaration **********************/

	/*
	 * Open the HDF file.
	 */
	file_id = Hopen(FILE_NAME, DFACC_READ, 0);
	if (file_id == FAIL)
		printf("\nUnable to open file %s for reading.\n", FILE_NAME);
	else {
		/*
		 * Initialize the AN interface.
		 */
		an_id = ANstart(file_id);
		if (an_id == FAIL)
			printf("\nCannot start annotation handling on the file %s.\n", FILE_NAME);
		else {
			/*
			 * Get the annotation information, e.g., the numbers of file labels, file
			 * descriptions, data labels, and data descriptions.
			 */
			status_n = ANfileinfo(an_id, &n_file_labels, &n_file_descs, &n_data_labels, &n_data_descs);
			if (status_n == FAIL)
				printf("\nCould not get the annotation information\n");
			else {
				/*
				 * Get the data labels.  Note that this for loop can be used to
				 * obtain the contents of each kind of annotation with the appropriate
				 * number of annotations and the type of annotation, i.e., replace
				 * n_data_labels with n_file_labels, n_file_descs, or n_data_descs, and
				 * AN_DATA_LABEL with AN_FILE_LABEL, AN_FILE_DESC, or AN_DATA_DESC,
				 * respectively.
				 */
				ret_val = SUCCEED;
				for (index = 0; index < n_data_labels; index++) {
					/*
					 * Get the identifier of the current data label.
					 */
					ann_id = ANselect(an_id, index, AN_DATA_LABEL);
					if (ann_id == FAIL) {
						printf("\nCannot get the identifier of the current data label.\n");
						ret_val = FAIL;
					}
					else {
						/*
						 * Get the length of the data label.
						 */
						ann_length = ANannlen(ann_id);
						if (ann_id == FAIL) {
							printf("\nCannot read annotation length.\n");
							ret_val = FAIL;
						}
						else {
							/*
							 * Allocate space for the buffer to hold the data label text.
							 */
							ann_buf = malloc((ann_length + 1) * sizeof(char));
							if (ann_buf) {
								/*
								 * Read and display the data label.  Note that the size of the buffer,
								 * i.e., the third parameter, is 1 character more than the length of
								 * the data label; that is for the null character.  It is not the case
								 * when a description is retrieved because the description does not
								 * necessarily end with a null character.
								 *
								 */
								status_32 = ANreadann(ann_id, ann_buf, ann_length + 1);
								if (status_32 == FAIL) {
									printf("\nCannot read annotation.\n");
									ret_val = FAIL;
								}
								else {
									printf("Data label index: %d\n", index);
									printf("Data label contents: %s\n", ann_buf);
								}
								/*
								 * Free the space allocated for the annotation buffer.
								 */
								free(ann_buf);
							}
							else {
								printf("\nAllocation failure\n");
							}
						}
						/*
						 * Terminate access to the current data label.
						 */
						status_n = ANendaccess(ann_id);
					}
				} /* for */
			}
			/*
			 * Terminate access to the AN interface and close the HDF file.
			 */
			status_32 = ANend(an_id);
		}
		status_n = Hclose(file_id);
	}
	if (ret_val == SUCCEED)
		printf("\nSuccessful!\n");
	return ret_val;
}
