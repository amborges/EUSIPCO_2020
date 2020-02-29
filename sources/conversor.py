from PIL import Image
import argparse
import math
import numpy
import sys


#GLOBAL CONTROL
FRAME_MATRIX = None #matrix containing all depth levels of the actual frame
FRAME_MATRIXP = None
IS_FIRST_FRAME = True

#SUB ROUTINES

def exportPNG(videowidth, videoheight, CQ, maxFrames, encoderName):
	global FRAME_MATRIX
	
	#DEFINING COLORS
	RED    = (255,0,0)		#depth 0 - 128x128	- int 15
	ORANGE = (255,165,0)	#depth 1 - 64x64	- int 12
	YELLOW = (255,255,0)	#depth 2 - 32x32	- int 9
	GREEN  = (0,255,0)		#depth 3 - 16x16	- int 6
	BLUE   = (0,0,255)		#depth 4 - 8x8		- int 3
	PURPLE = (128,0,128)	#depth 5 - 4x4		- int 0
	BLACK  = (0,0,0)		#depthERR- INVALID	- int 255
	
	max_col = int(videowidth / 4)
	max_row = int(videoheight / 4)

	for frm in range(maxFrames + 1):

		col_pos = 0
		row_pos = 0
	
		im = Image.new("RGB", (max_col, max_row))
		pix = im.load()
	
		for row in range(max_row):
			for col in range(max_col):
				if(FRAME_MATRIX[frm][col][row] == 0):
					pix[col,row] = RED
				elif(FRAME_MATRIX[frm][col][row] == 1):
					pix[col,row] = ORANGE
				elif(FRAME_MATRIX[frm][col][row] == 2):
					pix[col,row] = YELLOW
				elif(FRAME_MATRIX[frm][col][row] == 3):
					pix[col,row] = GREEN
				elif(FRAME_MATRIX[frm][col][row] == 4):
					pix[col,row] = BLUE
				elif(FRAME_MATRIX[frm][col][row] == 5):
					pix[col,row] = PURPLE
				else: #if(FRAME_MATRIX[0][col][row] == 6)
					pix[col,row] = BLACK
					
		im.save("PNG/"+encoderName+"_cq"+CQ+"_frame"+str(frm)+".png", "PNG")
	

def exportFrameMatrix(videowidth, videoheight, maxFrames, encoderName):
	global FRAME_MATRIX
	global FRAME_MATRIXP
	output = open('framematrix_'+encoderName+'.txt', 'a')
	
	max_col = int(videowidth / 4)
	max_row = int(videoheight / 4)

	for frm in range(maxFrames + 1):

		col_pos = 0
		row_pos = 0
		
		for row in range(0, max_row, 32):
			for col in range(0, max_col, 32):
				for r in range(row, row + 32):
					for c in range(col, col + 32):
						if(FRAME_MATRIXP[frm][c][r] == 3): #SPLIT CASE
							FRAME_MATRIXP[frm][c][r] = 0
							FRAME_MATRIX[frm][c][r] = FRAME_MATRIX[frm][c][r] + 1
						output.write(str(FRAME_MATRIX[frm][c][r]))
						output.write(str(FRAME_MATRIXP[frm][c][r]))
						#FRAME_MATRIX[frm][c][r] = 6 #ERASING
					output.write("\n")
				output.write("\n")
	
	output.close()

"""
 0 BLOCK_4X4 - DL 5
 1 BLOCK_4X8 - DL 4
 2 BLOCK_8X4 - DL 4
 3 BLOCK_8X8 - DL 4
 4 BLOCK_8X16 - DL 3
 5 BLOCK_16X8 - DL 3
 6 BLOCK_16X16 - DL 3
 7 BLOCK_16X32 - DL 2
 8 BLOCK_32X16 - DL 2
 9 BLOCK_32X32 - DL 2
10 BLOCK_32X64 - DL 1
11 BLOCK_64X32 - DL 1
12 BLOCK_64X64 - DL 1
13 BLOCK_64X128 - DL 0
14 BLOCK_128X64 - DL 0
15 BLOCK_128X128 - DL 0
16 BLOCK_4X16 - DL 3
17 BLOCK_16X4 - DL 3
18 BLOCK_8X32 - DL 2
19 BLOCK_32X8 - DL 2
20 BLOCK_16X64 - DL 1
21 BLOCK_64X16 - DL 1
"""


def splitLine(line):
	global IS_FIRST_FRAME
	
	if(line == ''):
		return (None, None, None, None, None)
		
	linesplitted = line.split("\t")
	
	frm = int(linesplitted[0])
	row = int(linesplitted[1])
	col = int(linesplitted[2])
	blk = int(linesplitted[3])
	prt = int(linesplitted[4])
	
	
	if(blk == 15): #128x128
		dln = 0
	elif(blk == 12): #64x64
		dln = 1
	elif(blk == 9): #32x32
		dln = 2
	elif(blk == 6): #16x16
		dln = 3
	elif(blk == 3): #8x8
		dln = 4
	elif(blk == 0): #4x4
		dln = 5
	elif(blk == 1 or blk == 2 or blk == 4 or blk == 5 or blk == 7 or blk == 8 or blk == 10 or blk == 11 or blk == 13 or blk == 14 or blk == 16 or blk == 17 or blk == 19 or blk == 20 or blk == 21):
		dln = 6 #invalid
		print("encerrando, bloco retangular (", blk, ")")
		return (None, None, None, None, None)
	else:
		dln = 6 #invalid
		print("encerrando, nenhum dos casos anteriores foi encontrado. Valor do bloco=", blk)
		return (None, None, None, None, None)
		
	return (dln, row, col, frm, prt)


def printMatrix(dln, col, row, frm, prt):
	global FRAME_MATRIX
	if(dln == 0):
		plusIterator = 32
	elif(dln == 1):
		plusIterator = 16
	elif(dln == 2):
		plusIterator = 8
	elif(dln == 3):
		plusIterator = 4
	elif(dln == 4):
		plusIterator = 2
	elif(dln == 5):
		plusIterator = 1
	elif(dln == 6):
		return
		
	for c in range(col, col + plusIterator):
		for r in range(row, row + plusIterator):
			FRAME_MATRIX[frm][c][r] = dln
			FRAME_MATRIXP[frm][c][r] = prt


#MAIN

parser = argparse.ArgumentParser(description='Ph.D. Python Code for conversion of AV1/VP9 Superblock structure to Depth Level matrices')
parser.add_argument('videoResolution', help='Video resolutions, only allows these values: 240p, 360p, 720p, 1080p, 2160p (UHD4K), and 4320p (UHD8K)')
parser.add_argument('CQ', help='QP of AV1/VP9')
parser.add_argument('encoder', help='AV1 or VP9')

args = parser.parse_args()

videoResolution = args.videoResolution
CQ = args.CQ
encoderName = args.encoder

if videoResolution == '240p':
	width = 426
	height = 240
elif videoResolution == '360p':
	width = 640
	height = 360
elif videoResolution == '720p':
	width = 1280
	height = 720
elif videoResolution == '1080p':
	width = 1920
	height = 1080
elif videoResolution == '2160p':
	width = 4096
	height = 2160
elif videoResolution == '4320p':
	width = 8192
	height = 4320
else:
	print("Fail to recognize the resolution")
	exit()

dlf = open('phd'+encoderName+'.sb')

colNumber = int(width / 4) + 32
rowNumber = int(height / 4) + 32
FRAME_MATRIX = [[[6 for y in range(rowNumber)] for x in range(colNumber)] for f in range(65)]
FRAME_MATRIXP= [[[6 for y in range(rowNumber)] for x in range(colNumber)] for f in range(65)]

print(encoderName," matrix create with size", colNumber, rowNumber)

av1_lastFrameVisited = -1
maxFrames = 0
vp9_frame_id = -1

while True:
	line = dlf.readline()
	(dln, row, col, frm, prt) = splitLine(line)
	
	if(dln == None):
		break
	
	if(encoderName == 'av1'):
		if(av1_lastFrameVisited != frm):
			av1_lastFrameVisited = frm
			if(maxFrames < frm):
				maxFrames = frm
			print(encoderName,"frame", frm)
		printMatrix(dln, col, row, frm, prt)
	elif(encoderName == 'vp9'):
		if(row == 0 and col == 0 and dln == 1):
			vp9_frame_id = vp9_frame_id + 1
			if(vp9_frame_id > 0):
				maxFrames = maxFrames + 1
			print(encoderName,"frame", vp9_frame_id)
		if(dln == 0):
			print("estranho")
		printMatrix(dln, col, row, vp9_frame_id, prt)

exportFrameMatrix(width, height, maxFrames, encoderName)
#exportPNG(width, height, CQ, maxFrames, encoderName)
