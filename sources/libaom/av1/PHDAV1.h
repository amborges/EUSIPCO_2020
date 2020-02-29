#ifndef PHDAV1_H
#define PHDAV1_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common/enums.h"

	//decode use, print the superblock partitioning
void phdav1_print(BLOCK_SIZE, PARTITION_TYPE, unsigned int, int, int);

	//encode use, modify the superblock partitioning
BLOCK_SIZE phdav1_newMAX(const int, const int, const BLOCK_SIZE, const int, const unsigned int, const int);
BLOCK_SIZE phdav1_newMIN(const int, const int, const BLOCK_SIZE, const int);
void phdav1_partitionChange(const int, const int, const int, int*, int*, int*, int*);

#endif // PHDAV1_H
