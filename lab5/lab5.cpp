#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define MROWS 100

int main(int argc, char** argv)
{
	int ret  = -1;	///< For return values
	int size = -1;	///< Total number of processors
	int rank = -1;	///< This processor's number

	const int count = 1e1; ///< Number of array elements
	// printf("DEFAULT VALUE - %d\n\n", DEBUG);
	const int random_seed = 1488; ///< RNG seed

	int* array = 0; ///< The array we need to find the max in
	int lmax = -1;	///< Local maximums
	int  max = -1;  ///< The maximal element

	double end, start;

	// * GENERATE TRACEFILE
    char filename[50];
    sprintf(filename, "trace.txt");
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Can't open file\n");
        exit(1);
    }

	/* Initialize the MPI */
	ret = MPI_Init(&argc, &argv);
	if (!rank) { printf("MPI Init returned (%d);\n", ret); }

	/* Determine our rankand processor count */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	// printf("MPI Comm Size: %d;\n", size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// printf("MPI Comm Rank: %d;\n", rank);

	/* Allocate the array */
	array = (int*)malloc(count * sizeof(int));

	// ! Master generates the array */
	if (!rank) {
		/* Initialize the RNG */
		srand(RANDOM_SEED);
		/* Generate the random array */
		for (int i = 0; i < count; i++) { array[i] = rand(); }
	}

	start = MPI_Wtime();

	//printf("Processor #%d has array: ", rank);
	//for (int i = 0; i < count; i++) { printf("%d ", array[i]); }
	//printf("\n");

	/* Send the array to all other processors */
	MPI_Bcast(array, count, MPI_INTEGER, 0, MPI_COMM_WORLD);

	// printf("Processor #%d has array: ", rank);
	// for (int i = 0; i < count; i++) { printf("%d ", array[i]); }
	// printf("\n");

	const int wstart = (rank    ) * count / size;
	const int wend   = (rank + 1) * count / size;

	// printf("Processor #%d checks items %d .. %d;\n", rank, wstart, wend - 1);

	for (int i = wstart;
		i < wend;
		i++)
	{
		if (array[i] > lmax) { lmax = array[i]; }
	}

	// printf("Processor #%d reports local max = %d;\n", rank, lmax);

	MPI_Reduce(&lmax, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);

	end = MPI_Wtime();
	ret = MPI_Finalize();
	// if (!rank) { 
	// 	printf("*** Global Maximum is %d;\n", max);
	// }

	fprintf(fp, "%lf;", end-start);
	// printf("MPI Finalize returned (%d);\n", ret);

	return(0);
}