#include <stdio.h>
#include <stdlib.h>

#define MAXAV1 22
#define MAXVP9 13

#define BLOCK_AV1 6
#define BLOCK_VP9 4 + 1 // a profundidade zero não conta

#define PART_AV1 10
#define PART_VP9 4

short block_av1(short depth, short partition){
	short block;
	switch(depth){
		case 0:
			switch(partition){
				case 0: block = 15; break;
				case 1: block = 13; break;
				case 2: block = 14; break;
			}
			break;
		case 1:
			switch(partition){
				case 0: block = 12; break;
				case 1: block = 10; break;
				case 2: block = 11; break;
				case 3: block = 20; break;
				case 4: block = 21; break;
			}
			break;
		case 2:
			switch(partition){
				case 0: block = 9; break;
				case 1: block = 7; break;
				case 2: block = 8; break;
				case 3: block = 18; break;
				case 4: block = 19; break;
			}
			break;
		case 3:
			switch(partition){
				case 0: block = 6; break;
				case 1: block = 4; break;
				case 2: block = 5; break;
				case 3: block = 16; break;
				case 4: block = 17; break;
			}
			break;
		case 4:
			switch(partition){
				case 0: block = 3; break;
				case 1: block = 1; break;
				case 2: block = 2; break;
			}
			break;
		case 5:
			block = 0;
			break;
		default:
			block = MAXAV1;
	}
	return block;
}

short block_vp9(short depth, short partition){
	short block;
	switch(depth){
		case 1:
			switch(partition){
				case 0: block = 12; break;
				case 1: block = 10; break;
				case 2: block = 11; break;
			}
			break;
		case 2:
			switch(partition){
				case 0: block = 9; break;
				case 1: block = 7; break;
				case 2: block = 8; break;
			}
			break;
		case 3:
			switch(partition){
				case 0: block = 6; break;
				case 1: block = 4; break;
				case 2: block = 5; break;
			}
			break;
		case 4:
			switch(partition){
				case 0: block = 3; break;
				case 1: block = 1; break;
				case 2: block = 2; break;
			}
			break;
		case 5:
			block = 0;
			break;
		default:
			block = MAXVP9;
	}
	return block;
}


int main(){
	FILE *av1, *vp9;
	av1 = fopen("framematrix_av1.txt", "r");
	vp9 = fopen("framematrix_vp9.txt", "r");
	
	unsigned long int TABLE[BLOCK_AV1 + 1][PART_AV1 + 1][BLOCK_VP9 + 1][PART_VP9 + 1];
	for(int b1 = 0; b1 <= BLOCK_AV1; b1++){
		for(int p1 = 0; p1 <= PART_AV1; p1++){
			for(int b9 = 0; b9 <= BLOCK_VP9; b9++){
				for(int p9 = 0; p9 <= PART_VP9; p9++){
					TABLE[b1][p1][b9][p9] = 0;
				}
			}
		}
	}
	char da, dv, pa, pv;
	short ia, iv, ipa, ipv, ba, bv;
	
	do{
		da = fgetc(av1);
		dv = fgetc(vp9);
		
		if(da == '\n')
			continue;
		
		pa= fgetc(av1);
		pv= fgetc(vp9);
		
		ia = da - 48;
		iv = dv - 48;
		
		ipa = pa - 48;
		ipv = pv - 48;
		
		//ba = block_av1(da, ipa);
		//bv = block_vp9(dv, ipv);
		
		TABLE[ia][ipa][iv][ipv]++;
	}while(da != EOF);
	
	//printf("SAIDA\n");
	/*
	printf("AV1\t\tVP9\n");
	printf("blk prt blk prt\n");
	for(int b1 = 0; b1 < BLOCK_AV1; b1++){
		for(int p1 = 0; p1 < PART_AV1; p1++){
			for(int b9 = 1; b9 < BLOCK_VP9; b9++){
				for(int p9 = 0; p9 < PART_VP9; p9++){
					//printf("AV1\tb[%i]\tp[%i]\tVP9\tb[%i]\tp[%i]\t%lu\n", b1, p1, b9, p9, TABLE[b1][p1][b9][p9]);
					printf("%i\t%i\t%i\t%i\t%lu\n", b1, p1, b9, p9, TABLE[b1][p1][b9][p9]);
				}
			}
		}
	}
	*/
	
	
	printf("AV1 \t\t VP9\n");
	printf("\t\t64x64 \t\t\t32x32 \t\t\t16x16 \t\t\t8x8 \n");
	printf("\t\tNone \tHorz \tVert \tNone \tHorz \tVert \tNone \tHorz \tVert \tNone \tHorz \tVert \n");
	
	//--------------------//
	
	printf("128x128 \tNone \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[0][0][1][0], TABLE[0][0][1][1], TABLE[0][0][1][2],
	TABLE[0][0][2][0], TABLE[0][0][2][1], TABLE[0][0][2][2],
	TABLE[0][0][3][0], TABLE[0][0][3][1], TABLE[0][0][3][2],
	TABLE[0][0][4][0], TABLE[0][0][4][1], TABLE[0][0][4][2]);
	
	printf("\tHorz \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[0][1][1][0] + TABLE[0][4][1][0] + TABLE[0][5][1][0] + TABLE[0][8][1][0], 
	TABLE[0][1][1][1] + TABLE[0][4][1][1] + TABLE[0][5][1][1] + TABLE[0][8][1][1], 
	TABLE[0][1][1][2] + TABLE[0][4][1][2] + TABLE[0][5][1][2] + TABLE[0][8][1][2],
	
	TABLE[0][1][2][0] + TABLE[0][4][2][0] + TABLE[0][5][2][0] + TABLE[0][8][2][0], 
	TABLE[0][1][2][1] + TABLE[0][4][2][1] + TABLE[0][5][2][1] + TABLE[0][8][2][1], 
	TABLE[0][1][2][2] + TABLE[0][4][2][2] + TABLE[0][5][2][2] + TABLE[0][8][2][2],
	
	TABLE[0][1][3][0] + TABLE[0][4][3][0] + TABLE[0][5][3][0] + TABLE[0][8][3][0], 
	TABLE[0][1][3][1] + TABLE[0][4][3][1] + TABLE[0][5][3][1] + TABLE[0][8][3][1], 
	TABLE[0][1][3][2] + TABLE[0][4][3][2] + TABLE[0][5][3][2] + TABLE[0][8][3][2],
	
	TABLE[0][1][4][0] + TABLE[0][4][4][0] + TABLE[0][5][4][0] + TABLE[0][8][4][0], 
	TABLE[0][1][4][1] + TABLE[0][4][4][1] + TABLE[0][5][4][1] + TABLE[0][8][4][1], 
	TABLE[0][1][4][2] + TABLE[0][4][4][2] + TABLE[0][5][4][2] + TABLE[0][8][4][2]);
	
	printf("\tVert \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[0][2][1][0] + TABLE[0][6][1][0] + TABLE[0][7][1][0] + TABLE[0][9][1][0], 
	TABLE[0][2][1][1] + TABLE[0][6][1][1] + TABLE[0][7][1][1] + TABLE[0][9][1][1], 
	TABLE[0][2][1][2] + TABLE[0][6][1][2] + TABLE[0][7][1][2] + TABLE[0][9][1][2],
	
	TABLE[0][2][2][0] + TABLE[0][6][2][0] + TABLE[0][7][2][0] + TABLE[0][9][2][0], 
	TABLE[0][2][2][1] + TABLE[0][6][2][1] + TABLE[0][7][2][1] + TABLE[0][9][2][1], 
	TABLE[0][2][2][2] + TABLE[0][6][2][2] + TABLE[0][7][2][2] + TABLE[0][9][2][2],
	
	TABLE[0][2][3][0] + TABLE[0][6][3][0] + TABLE[0][7][3][0] + TABLE[0][9][3][0], 
	TABLE[0][2][3][1] + TABLE[0][6][3][1] + TABLE[0][7][3][1] + TABLE[0][9][3][1], 
	TABLE[0][2][3][2] + TABLE[0][6][3][2] + TABLE[0][7][3][2] + TABLE[0][9][3][2],
	
	TABLE[0][2][4][0] + TABLE[0][6][4][0] + TABLE[0][7][4][0] + TABLE[0][9][4][0], 
	TABLE[0][2][4][1] + TABLE[0][6][4][1] + TABLE[0][7][4][1] + TABLE[0][9][4][1], 
	TABLE[0][2][4][2] + TABLE[0][6][4][2] + TABLE[0][7][4][2] + TABLE[0][9][4][2]);
	
	
	//--------------------//
	
	printf("64x64 \tNone \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[1][0][1][0], TABLE[1][0][1][1], TABLE[1][0][1][2],
	TABLE[1][0][2][0], TABLE[1][0][2][1], TABLE[1][0][2][2],
	TABLE[1][0][3][0], TABLE[1][0][3][1], TABLE[1][0][3][2],
	TABLE[1][0][4][0], TABLE[1][0][4][1], TABLE[1][0][4][2]);
	
	printf("\tHorz \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[1][1][1][0] + TABLE[1][4][1][0] + TABLE[1][5][1][0] + TABLE[1][8][1][0], 
	TABLE[1][1][1][1] + TABLE[1][4][1][1] + TABLE[1][5][1][1] + TABLE[1][8][1][1], 
	TABLE[1][1][1][2] + TABLE[1][4][1][2] + TABLE[1][5][1][2] + TABLE[1][8][1][2],
	
	TABLE[1][1][2][0] + TABLE[1][4][2][0] + TABLE[1][5][2][0] + TABLE[1][8][2][0], 
	TABLE[1][1][2][1] + TABLE[1][4][2][1] + TABLE[1][5][2][1] + TABLE[1][8][2][1], 
	TABLE[1][1][2][2] + TABLE[1][4][2][2] + TABLE[1][5][2][2] + TABLE[1][8][2][2],
	
	TABLE[1][1][3][0] + TABLE[1][4][3][0] + TABLE[1][5][3][0] + TABLE[1][8][3][0], 
	TABLE[1][1][3][1] + TABLE[1][4][3][1] + TABLE[1][5][3][1] + TABLE[1][8][3][1], 
	TABLE[1][1][3][2] + TABLE[1][4][3][2] + TABLE[1][5][3][2] + TABLE[1][8][3][2],
	
	TABLE[1][1][4][0] + TABLE[1][4][4][0] + TABLE[1][5][4][0] + TABLE[1][8][4][0], 
	TABLE[1][1][4][1] + TABLE[1][4][4][1] + TABLE[1][5][4][1] + TABLE[1][8][4][1], 
	TABLE[1][1][4][2] + TABLE[1][4][4][2] + TABLE[1][5][4][2] + TABLE[1][8][4][2]);
	
	printf("\tVert \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[1][2][1][0] + TABLE[1][6][1][0] + TABLE[1][7][1][0] + TABLE[1][9][1][0], 
	TABLE[1][2][1][1] + TABLE[1][6][1][1] + TABLE[1][7][1][1] + TABLE[1][9][1][1], 
	TABLE[1][2][1][2] + TABLE[1][6][1][2] + TABLE[1][7][1][2] + TABLE[1][9][1][2],
	
	TABLE[1][2][2][0] + TABLE[1][6][2][0] + TABLE[1][7][2][0] + TABLE[1][9][2][0], 
	TABLE[1][2][2][1] + TABLE[1][6][2][1] + TABLE[1][7][2][1] + TABLE[1][9][2][1], 
	TABLE[1][2][2][2] + TABLE[1][6][2][2] + TABLE[1][7][2][2] + TABLE[1][9][2][2],
	
	TABLE[1][2][3][0] + TABLE[1][6][3][0] + TABLE[1][7][3][0] + TABLE[1][9][3][0], 
	TABLE[1][2][3][1] + TABLE[1][6][3][1] + TABLE[1][7][3][1] + TABLE[1][9][3][1], 
	TABLE[1][2][3][2] + TABLE[1][6][3][2] + TABLE[1][7][3][2] + TABLE[1][9][3][2],
	
	TABLE[1][2][4][0] + TABLE[1][6][4][0] + TABLE[1][7][4][0] + TABLE[1][9][4][0], 
	TABLE[1][2][4][1] + TABLE[1][6][4][1] + TABLE[1][7][4][1] + TABLE[1][9][4][1], 
	TABLE[1][2][4][2] + TABLE[1][6][4][2] + TABLE[1][7][4][2] + TABLE[1][9][4][2]);
	
	
	//--------------------//
	
	printf("32x32 \tNone \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[2][0][1][0], TABLE[2][0][1][1], TABLE[2][0][1][2],
	TABLE[2][0][2][0], TABLE[2][0][2][1], TABLE[2][0][2][2],
	TABLE[2][0][3][0], TABLE[2][0][3][1], TABLE[2][0][3][2],
	TABLE[2][0][4][0], TABLE[2][0][4][1], TABLE[2][0][4][2]);
	
	printf("\tHorz \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[2][1][1][0] + TABLE[2][4][1][0] + TABLE[2][5][1][0] + TABLE[2][8][1][0], 
	TABLE[2][1][1][1] + TABLE[2][4][1][1] + TABLE[2][5][1][1] + TABLE[2][8][1][1], 
	TABLE[2][1][1][2] + TABLE[2][4][1][2] + TABLE[2][5][1][2] + TABLE[2][8][1][2],
	
	TABLE[2][1][2][0] + TABLE[2][4][2][0] + TABLE[2][5][2][0] + TABLE[2][8][2][0], 
	TABLE[2][1][2][1] + TABLE[2][4][2][1] + TABLE[2][5][2][1] + TABLE[2][8][2][1], 
	TABLE[2][1][2][2] + TABLE[2][4][2][2] + TABLE[2][5][2][2] + TABLE[2][8][2][2],
	
	TABLE[2][1][3][0] + TABLE[2][4][3][0] + TABLE[2][5][3][0] + TABLE[2][8][3][0], 
	TABLE[2][1][3][1] + TABLE[2][4][3][1] + TABLE[2][5][3][1] + TABLE[2][8][3][1], 
	TABLE[2][1][3][2] + TABLE[2][4][3][2] + TABLE[2][5][3][2] + TABLE[2][8][3][2],
	
	TABLE[2][1][4][0] + TABLE[2][4][4][0] + TABLE[2][5][4][0] + TABLE[2][8][4][0], 
	TABLE[2][1][4][1] + TABLE[2][4][4][1] + TABLE[2][5][4][1] + TABLE[2][8][4][1], 
	TABLE[2][1][4][2] + TABLE[2][4][4][2] + TABLE[2][5][4][2] + TABLE[2][8][4][2]);
	
	printf("\tVert \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[2][2][1][0] + TABLE[2][6][1][0] + TABLE[2][7][1][0] + TABLE[2][9][1][0], 
	TABLE[2][2][1][1] + TABLE[2][6][1][1] + TABLE[2][7][1][1] + TABLE[2][9][1][1], 
	TABLE[2][2][1][2] + TABLE[2][6][1][2] + TABLE[2][7][1][2] + TABLE[2][9][1][2],
	
	TABLE[2][2][2][0] + TABLE[2][6][2][0] + TABLE[2][7][2][0] + TABLE[2][9][2][0], 
	TABLE[2][2][2][1] + TABLE[2][6][2][1] + TABLE[2][7][2][1] + TABLE[2][9][2][1], 
	TABLE[2][2][2][2] + TABLE[2][6][2][2] + TABLE[2][7][2][2] + TABLE[2][9][2][2],
	
	TABLE[2][2][3][0] + TABLE[2][6][3][0] + TABLE[2][7][3][0] + TABLE[2][9][3][0], 
	TABLE[2][2][3][1] + TABLE[2][6][3][1] + TABLE[2][7][3][1] + TABLE[2][9][3][1], 
	TABLE[2][2][3][2] + TABLE[2][6][3][2] + TABLE[2][7][3][2] + TABLE[2][9][3][2],
	
	TABLE[2][2][4][0] + TABLE[2][6][4][0] + TABLE[2][7][4][0] + TABLE[2][9][4][0], 
	TABLE[2][2][4][1] + TABLE[2][6][4][1] + TABLE[2][7][4][1] + TABLE[2][9][4][1], 
	TABLE[2][2][4][2] + TABLE[2][6][4][2] + TABLE[2][7][4][2] + TABLE[2][9][4][2]);
	
	
	//--------------------//
	
	printf("16x16 \tNone \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[3][0][1][0], TABLE[3][0][1][1], TABLE[3][0][1][2],
	TABLE[3][0][2][0], TABLE[3][0][2][1], TABLE[3][0][2][2],
	TABLE[3][0][3][0], TABLE[3][0][3][1], TABLE[3][0][3][2],
	TABLE[3][0][4][0], TABLE[3][0][4][1], TABLE[3][0][4][2]);
	
	printf("\tHorz \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[3][1][1][0] + TABLE[3][4][1][0] + TABLE[3][5][1][0] + TABLE[3][8][1][0], 
	TABLE[3][1][1][1] + TABLE[3][4][1][1] + TABLE[3][5][1][1] + TABLE[3][8][1][1], 
	TABLE[3][1][1][2] + TABLE[3][4][1][2] + TABLE[3][5][1][2] + TABLE[3][8][1][2],
	
	TABLE[3][1][2][0] + TABLE[3][4][2][0] + TABLE[3][5][2][0] + TABLE[3][8][2][0], 
	TABLE[3][1][2][1] + TABLE[3][4][2][1] + TABLE[3][5][2][1] + TABLE[3][8][2][1], 
	TABLE[3][1][2][2] + TABLE[3][4][2][2] + TABLE[3][5][2][2] + TABLE[3][8][2][2],
	
	TABLE[3][1][3][0] + TABLE[3][4][3][0] + TABLE[3][5][3][0] + TABLE[3][8][3][0], 
	TABLE[3][1][3][1] + TABLE[3][4][3][1] + TABLE[3][5][3][1] + TABLE[3][8][3][1], 
	TABLE[3][1][3][2] + TABLE[3][4][3][2] + TABLE[3][5][3][2] + TABLE[3][8][3][2],
	
	TABLE[3][1][4][0] + TABLE[3][4][4][0] + TABLE[3][5][4][0] + TABLE[3][8][4][0], 
	TABLE[3][1][4][1] + TABLE[3][4][4][1] + TABLE[3][5][4][1] + TABLE[3][8][4][1], 
	TABLE[3][1][4][2] + TABLE[3][4][4][2] + TABLE[3][5][4][2] + TABLE[3][8][4][2]);
	
	printf("\tVert \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[3][2][1][0] + TABLE[3][6][1][0] + TABLE[3][7][1][0] + TABLE[3][9][1][0], 
	TABLE[3][2][1][1] + TABLE[3][6][1][1] + TABLE[3][7][1][1] + TABLE[3][9][1][1], 
	TABLE[3][2][1][2] + TABLE[3][6][1][2] + TABLE[3][7][1][2] + TABLE[3][9][1][2],
	
	TABLE[3][2][2][0] + TABLE[3][6][2][0] + TABLE[3][7][2][0] + TABLE[3][9][2][0], 
	TABLE[3][2][2][1] + TABLE[3][6][2][1] + TABLE[3][7][2][1] + TABLE[3][9][2][1], 
	TABLE[3][2][2][2] + TABLE[3][6][2][2] + TABLE[3][7][2][2] + TABLE[3][9][2][2],
	
	TABLE[3][2][3][0] + TABLE[3][6][3][0] + TABLE[3][7][3][0] + TABLE[3][9][3][0], 
	TABLE[3][2][3][1] + TABLE[3][6][3][1] + TABLE[3][7][3][1] + TABLE[3][9][3][1], 
	TABLE[3][2][3][2] + TABLE[3][6][3][2] + TABLE[3][7][3][2] + TABLE[3][9][3][2],
	
	TABLE[3][2][4][0] + TABLE[3][6][4][0] + TABLE[3][7][4][0] + TABLE[3][9][4][0], 
	TABLE[3][2][4][1] + TABLE[3][6][4][1] + TABLE[3][7][4][1] + TABLE[3][9][4][1], 
	TABLE[3][2][4][2] + TABLE[3][6][4][2] + TABLE[3][7][4][2] + TABLE[3][9][4][2]);
	
	
	
	//--------------------//
	
	printf("8x8 \tNone \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[4][0][1][0], TABLE[4][0][1][1], TABLE[4][0][1][2],
	TABLE[4][0][2][0], TABLE[4][0][2][1], TABLE[4][0][2][2],
	TABLE[4][0][3][0], TABLE[4][0][3][1], TABLE[4][0][3][2],
	TABLE[4][0][4][0], TABLE[4][0][4][1], TABLE[4][0][4][2]);
	
	printf("\tHorz \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[4][1][1][0] + TABLE[4][4][1][0] + TABLE[4][5][1][0] + TABLE[4][8][1][0], 
	TABLE[4][1][1][1] + TABLE[4][4][1][1] + TABLE[4][5][1][1] + TABLE[4][8][1][1], 
	TABLE[4][1][1][2] + TABLE[4][4][1][2] + TABLE[4][5][1][2] + TABLE[4][8][1][2],
	
	TABLE[4][1][2][0] + TABLE[4][4][2][0] + TABLE[4][5][2][0] + TABLE[4][8][2][0], 
	TABLE[4][1][2][1] + TABLE[4][4][2][1] + TABLE[4][5][2][1] + TABLE[4][8][2][1], 
	TABLE[4][1][2][2] + TABLE[4][4][2][2] + TABLE[4][5][2][2] + TABLE[4][8][2][2],
	
	TABLE[4][1][3][0] + TABLE[4][4][3][0] + TABLE[4][5][3][0] + TABLE[4][8][3][0], 
	TABLE[4][1][3][1] + TABLE[4][4][3][1] + TABLE[4][5][3][1] + TABLE[4][8][3][1], 
	TABLE[4][1][3][2] + TABLE[4][4][3][2] + TABLE[4][5][3][2] + TABLE[4][8][3][2],
	
	TABLE[4][1][4][0] + TABLE[4][4][4][0] + TABLE[4][5][4][0] + TABLE[4][8][4][0], 
	TABLE[4][1][4][1] + TABLE[4][4][4][1] + TABLE[4][5][4][1] + TABLE[4][8][4][1], 
	TABLE[4][1][4][2] + TABLE[4][4][4][2] + TABLE[4][5][4][2] + TABLE[4][8][4][2]);
	
	printf("\tVert \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[4][2][1][0] + TABLE[4][6][1][0] + TABLE[4][7][1][0] + TABLE[4][9][1][0], 
	TABLE[4][2][1][1] + TABLE[4][6][1][1] + TABLE[4][7][1][1] + TABLE[4][9][1][1], 
	TABLE[4][2][1][2] + TABLE[4][6][1][2] + TABLE[4][7][1][2] + TABLE[4][9][1][2],
	
	TABLE[4][2][2][0] + TABLE[4][6][2][0] + TABLE[4][7][2][0] + TABLE[4][9][2][0], 
	TABLE[4][2][2][1] + TABLE[4][6][2][1] + TABLE[4][7][2][1] + TABLE[4][9][2][1], 
	TABLE[4][2][2][2] + TABLE[4][6][2][2] + TABLE[4][7][2][2] + TABLE[4][9][2][2],
	
	TABLE[4][2][3][0] + TABLE[4][6][3][0] + TABLE[4][7][3][0] + TABLE[4][9][3][0], 
	TABLE[4][2][3][1] + TABLE[4][6][3][1] + TABLE[4][7][3][1] + TABLE[4][9][3][1], 
	TABLE[4][2][3][2] + TABLE[4][6][3][2] + TABLE[4][7][3][2] + TABLE[4][9][3][2],
	
	TABLE[4][2][4][0] + TABLE[4][6][4][0] + TABLE[4][7][4][0] + TABLE[4][9][4][0], 
	TABLE[4][2][4][1] + TABLE[4][6][4][1] + TABLE[4][7][4][1] + TABLE[4][9][4][1], 
	TABLE[4][2][4][2] + TABLE[4][6][4][2] + TABLE[4][7][4][2] + TABLE[4][9][4][2]);
	
	
	//--------------------//
	
	printf("4x4 \tNone \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[5][0][1][0], TABLE[5][0][1][1], TABLE[5][0][1][2],
	TABLE[5][0][2][0], TABLE[5][0][2][1], TABLE[5][0][2][2],
	TABLE[5][0][3][0], TABLE[5][0][3][1], TABLE[5][0][3][2],
	TABLE[5][0][4][0], TABLE[5][0][4][1], TABLE[5][0][4][2]);
	
	printf("\tHorz \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[5][1][1][0] + TABLE[5][4][1][0] + TABLE[5][5][1][0] + TABLE[5][8][1][0], 
	TABLE[5][1][1][1] + TABLE[5][4][1][1] + TABLE[5][5][1][1] + TABLE[5][8][1][1], 
	TABLE[5][1][1][2] + TABLE[5][4][1][2] + TABLE[5][5][1][2] + TABLE[5][8][1][2],
	
	TABLE[5][1][2][0] + TABLE[5][4][2][0] + TABLE[5][5][2][0] + TABLE[5][8][2][0], 
	TABLE[5][1][2][1] + TABLE[5][4][2][1] + TABLE[5][5][2][1] + TABLE[5][8][2][1], 
	TABLE[5][1][2][2] + TABLE[5][4][2][2] + TABLE[5][5][2][2] + TABLE[5][8][2][2],
	
	TABLE[5][1][3][0] + TABLE[5][4][3][0] + TABLE[5][5][3][0] + TABLE[5][8][3][0], 
	TABLE[5][1][3][1] + TABLE[5][4][3][1] + TABLE[5][5][3][1] + TABLE[5][8][3][1], 
	TABLE[5][1][3][2] + TABLE[5][4][3][2] + TABLE[5][5][3][2] + TABLE[5][8][3][2],
	
	TABLE[5][1][4][0] + TABLE[5][4][4][0] + TABLE[5][5][4][0] + TABLE[5][8][4][0], 
	TABLE[5][1][4][1] + TABLE[5][4][4][1] + TABLE[5][5][4][1] + TABLE[5][8][4][1], 
	TABLE[5][1][4][2] + TABLE[5][4][4][2] + TABLE[5][5][4][2] + TABLE[5][8][4][2]);
	
	printf("\tVert \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu \t%lu\n",
	TABLE[5][2][1][0] + TABLE[5][6][1][0] + TABLE[5][7][1][0] + TABLE[5][9][1][0], 
	TABLE[5][2][1][1] + TABLE[5][6][1][1] + TABLE[5][7][1][1] + TABLE[5][9][1][1], 
	TABLE[5][2][1][2] + TABLE[5][6][1][2] + TABLE[5][7][1][2] + TABLE[5][9][1][2],
	
	TABLE[5][2][2][0] + TABLE[5][6][2][0] + TABLE[5][7][2][0] + TABLE[5][9][2][0], 
	TABLE[5][2][2][1] + TABLE[5][6][2][1] + TABLE[5][7][2][1] + TABLE[5][9][2][1], 
	TABLE[5][2][2][2] + TABLE[5][6][2][2] + TABLE[5][7][2][2] + TABLE[5][9][2][2],
	
	TABLE[5][2][3][0] + TABLE[5][6][3][0] + TABLE[5][7][3][0] + TABLE[5][9][3][0], 
	TABLE[5][2][3][1] + TABLE[5][6][3][1] + TABLE[5][7][3][1] + TABLE[5][9][3][1], 
	TABLE[5][2][3][2] + TABLE[5][6][3][2] + TABLE[5][7][3][2] + TABLE[5][9][3][2],
	
	TABLE[5][2][4][0] + TABLE[5][6][4][0] + TABLE[5][7][4][0] + TABLE[5][9][4][0], 
	TABLE[5][2][4][1] + TABLE[5][6][4][1] + TABLE[5][7][4][1] + TABLE[5][9][4][1], 
	TABLE[5][2][4][2] + TABLE[5][6][4][2] + TABLE[5][7][4][2] + TABLE[5][9][4][2]);
	
	
	fclose(vp9);
	fclose(av1);
	
	return 0;
}


/*

VP9
0 - PARTITION_NONE
1 - PARTITION_HORZ
2 - PARTITION_VERT
3 - PARTITION_SPLIT

AV1
0 - PARTITION_NONE
1 - PARTITION_HORZ
2 - PARTITION_VERT
3 - PARTITION_SPLIT
4 - PARTITION_HORZ_A
5 - PARTITION_HORZ_B
6 - PARTITION_VERT_A
7 - PARTITION_VERT_B
8 - PARTITION_HORZ_4
9 - PARTITION_VERT_4

*/
