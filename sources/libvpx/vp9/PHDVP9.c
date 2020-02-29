#include "PHDVP9.h"

void phdvp9_print(BLOCK_SIZE bsize, PARTITION_TYPE partition, unsigned int frame_number, int mi_row, int mi_col){
	/*
	if(bsize == BLOCK_64X64){
		printf("\n");
	}
	if(partition != PARTITION_SPLIT){
		switch(bsize){
			case BLOCK_4X4:   printf("b 4x4\t");   break;
			case BLOCK_4X8:   printf("b 4x8\t");   break;
			case BLOCK_8X4:   printf("b 8x4\t");   break;
			case BLOCK_8X8:   printf("b 8x8\t");   break;
			case BLOCK_8X16:  printf("b 8x16\t");  break;
			case BLOCK_16X8:  printf("b 16x8\t");  break;
			case BLOCK_16X16: printf("b 16x16\t"); break;
			case BLOCK_16X32: printf("b 16x32\t"); break;
			case BLOCK_32X16: printf("b 32x16\t"); break;
			case BLOCK_32X32: printf("b 32x32\t"); break;
			case BLOCK_32X64: printf("b 32x64\t"); break;
			case BLOCK_64X32: printf("b 64x32\t"); break;
			case BLOCK_64X64: printf("b 64x64\t"); break;
		}
		switch(partition){
			case PARTITION_NONE: printf("p none\t"); break;
			case PARTITION_HORZ: printf("p horz\t"); break;
			case PARTITION_VERT: printf("p vert\t"); break;
			case PARTITION_SPLIT: printf("p split\t"); break;
		}
		printf("f: %u\tpos[%d %d]\n", frame_number, mi_row, mi_col);
	}
	*/
	//if(partition != PARTITION_SPLIT){
		printf("%u\t%d\t%d\t%d\t%d\n", frame_number, mi_row * 2, mi_col * 2, bsize, partition);
	//}
}
