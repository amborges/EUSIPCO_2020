#include "PHDAV1.h"

/*
	Olhar com calma em rd_pick_partition
	try_intra_cnn_split
	try_split_only
	try_prune_rect
	
	min_sq_part
	max_sq_part
*/


/*
	GLOBAL VARIABLES OF PHDAV1.c
*/

const BLOCK_SIZE PHDAV1_BLOCK_SIZE_SQ_STEP = 3; //distance from every square block
BLOCK_SIZE PHDAV1_T_MATRIX[32][32]; //superblock matrix used for fast transcode
PARTITION_TYPE PHDAV1_T_MATRIXP[32][32]; //superblock matrix used for fast transcode
bool PHDAV1_FILE_WAS_OPEN = false;
FILE* T_MATRIX; //Transcode Matrix file from VP9
unsigned int LAST_FRAME_ID = -1;

BLOCK_SIZE PHDAV1_iMAX;
BLOCK_SIZE PHDAV1_iMIN;


/*
	DECODE
*/

void phdav1_print(BLOCK_SIZE bsize, PARTITION_TYPE partition, unsigned int frame_id, int mi_row, int mi_col){
	if(partition != PARTITION_SPLIT){
		printf("%u\t%d\t%d\t%d\t%d\n", frame_id, mi_row, mi_col, bsize, partition);
	}
}

/*
	ENCODE
*/

void phdav1_open_extern_file(){
	T_MATRIX = fopen("framematrix_vp9.txt", "r");
	PHDAV1_FILE_WAS_OPEN = true;
}

void phdav1_rewind_file_reader(int video_height){
	//there are cases that AV1 process the same frame_id number
	//in these cases, it is necessary to rewind the file position
	long int offset; //EQUATION: roundup(height/128)*roundup(width/128)*1024;
					 //1024 because the PHDAV1_T_MATRIX is 32*32
		 if(video_height == 240)  offset = -8192;   //SD240,  426x240
	else if(video_height == 360)  offset = -15360;   //SD360,  640x360
	else if(video_height == 720)  offset = -61440;   //HD720,  1280x720
	else if(video_height == 1080) offset = -138240;  //HD1080, 1920x1080
	else if(video_height == 2160) offset = -557056;  //UHD4K,  4096x2160
	else if(video_height == 4380) offset = -2228224; //UHD8K,  8192x4320

	fseek(T_MATRIX, offset, SEEK_CUR);
}

static unsigned int PHDAV1_CONTA = 0;
static unsigned int PHDAV1_BFCONTA = 0;

BLOCK_SIZE phdav1_getBlockFromFile(){
	PHDAV1_CONTA++;
	
	char c = fgetc(T_MATRIX);
	if(c == NULL){
		printf("Unfortunatelly there is no more chars in the file\n");
		exit(102);
	}
		
	if(c == '\n')
		return phdav1_getBlockFromFile();
	else{
		switch(c){
			case '0': return BLOCK_128X128; //it did not expected
			case '1': return BLOCK_64X64;
			case '2': return BLOCK_32X32;
			case '3': return BLOCK_16X16;
			case '4': return BLOCK_8X8;
			case '5': return BLOCK_4X4; //it did not expected
			case '6': return BLOCK_INVALID; //it is expected at border frame
			default: 
				//printf("\nWas found an invalid char in the file transcoder: %d\n", c);
				return BLOCK_INVALID;
		}
	}
}

BLOCK_SIZE phdav1_getPartitionFromFile(){
	//PHDAV1_CONTA++;
	
	char c = fgetc(T_MATRIX);
	if(c == NULL){
		printf("Unfortunatelly there is no more chars in the file\n");
		exit(102);
	}
	
	switch(c){
		case '0': return PARTITION_NONE;
		case '1': return PARTITION_HORZ;
		case '2': return PARTITION_VERT;
		default: 
			return PARTITION_INVALID;
	}
}


void phdav1_updateTranscodeMatrix(int mi_col, int mi_row, unsigned int frame_id, int video_height){
	if(!PHDAV1_FILE_WAS_OPEN){
		phdav1_open_extern_file();
	}
	
	if(mi_col == 0 && mi_row == 0){
		if(LAST_FRAME_ID == frame_id){
			phdav1_rewind_file_reader(video_height);
		}
		LAST_FRAME_ID = frame_id;
	}
	PHDAV1_iMAX = BLOCK_4X4;
	PHDAV1_iMIN = BLOCK_128X128;
	for(int row = 0; row < 32; row++){
		for(int col = 0; col < 32; col++){
			PHDAV1_T_MATRIX[col][row] = phdav1_getBlockFromFile();
			PHDAV1_T_MATRIXP[col][row] = phdav1_getPartitionFromFile();
			//if(PHDAV1_T_MATRIX[col][row] == BLOCK_INVALID){
				//printf("\nINVALID AT %d[%d, %d]\n", frame_id, mi_col*4, mi_row*4);
			//}
			//else{
			//	if(PHDAV1_iMAX < PHDAV1_T_MATRIX[col][row])
			//		PHDAV1_iMAX = PHDAV1_T_MATRIX[col][row];
			//	if(PHDAV1_iMIN > PHDAV1_T_MATRIX[col][row])
			//		PHDAV1_iMIN = PHDAV1_T_MATRIX[col][row];
			//}
		}
	}
	//printf("\n[%d, %d]\n", PHDAV1_iMAX, PHDAV1_iMIN);
}

BLOCK_SIZE phdav1_newMAX(const int mi_col, const int mi_row, const BLOCK_SIZE bsize, const int actualMax, const unsigned int frame_id, const int video_height){
	// to posicioning the new frame && mi_col == 0 && mi_row == 0
	if(bsize == BLOCK_128X128){
		//it is a new superblock
		phdav1_updateTranscodeMatrix(mi_col, mi_row, frame_id, video_height);
	}
	return actualMax;
	//return PHDAV1_iMAX;
	//return BLOCK_128X128;
}

BLOCK_SIZE phdav1_newMIN(const int mi_col, const int mi_row, const int actualMin){	
	return actualMin;
	//return PHDAV1_iMIN;
	//return BLOCK_4X4;
}

void phdav1_partitionChange(const int bsize, const int mi_col, const int mi_row, int* none, int* horz, int* vert){

	//the cases of BLOCK_4X4 and BLOCK_8X8 or equivalents in AV1
	if(bsize < BLOCK_8X8) return;
	
	if(bsize == BLOCK_128X128){
		if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_64X64){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_32X32){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_16X16){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_8X8){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}	
		}
	}
	else if(bsize == BLOCK_64X64){
		if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_64X64){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_32X32){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_16X16){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}	
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_8X8){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}	
		}
	}
	else if(bsize == BLOCK_32X32){
		if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_64X64){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_32X32){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 0;
				*horz = 1;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_16X16){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 0;
				*horz = 0;
				*vert = 1;
			}	
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_8X8){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}	
		}
	}
	else if(bsize == BLOCK_16X16){
		if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_64X64){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_32X32){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_16X16){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 0;
				*horz = 1;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}	
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_8X8){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}	
		}
	}
	else if(bsize == BLOCK_8X8){
		if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_64X64){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_32X32){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
				
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_16X16){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}	
		}
		else if(PHDAV1_T_MATRIX[mi_col % 32][mi_row % 32] == BLOCK_8X8){
			if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_NONE){
				*none = 1;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_HORZ){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}
			else if(PHDAV1_T_MATRIXP[mi_col % 32][mi_row % 32] == PARTITION_VERT){
				*none = 0;
				*horz = 0;
				*vert = 0;
			}	
		}
	}
}

