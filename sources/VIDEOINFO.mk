#Informacoes Compartilhadas entre os videos
FTBE = 60
CQs  = 20 32 43 55


CFG_GENERAL = --verbose --psnr
AV1_COMMON  = $(CFG_GENERAL) --frame-parallel=0 --tile-columns=0 --cpu-used=0 --threads=1

#High Latency CQP [Ideal para comparar AV1 com codificadores semelhantes na melhor qualidade]
AV1_HLCQP = $(AV1_COMMON) --end-usage=q --auto-alt-ref=2 #--cq-level={22,32,43,55}
#Low Latency CQP [Ideal para comparar AV1 com codificadores semelhantes numa qualidade restrita]
AV1_LLCQP = $(AV1_COMMON) --end-usage=q --lag-in-frames=0 #--cq-level={22,32,43,55}
#Unconstrained High Latency [Ideal para executar o AV1 em sua melhor qualidade]
AV1_UHL_CQ = $(AV1_COMMON) --end-usage=q --lag-in-frames=25 --auto-alt-ref=2 #--cq-level={22,32,43,55}
AV1_UHL_VBR = $(AV1_COMMON) --end-usage=vbr --lag-in-frames=25 --auto-alt-ref=2 #--target-bitrate={arbitrário}
#Unconstrained Low Latency []
AV1_ULL_CQ = $(AV1_COMMON) --end-usage=q --lag-in-frames=0 #--cq-level={22,32,43,55}
AV1_ULL_VBR = $(AV1_COMMON) --end-usage=vbr --lag-in-frames=0 #--target-bitrate={arbitrário}

AV1_10B = --input-bit-depth=10 --bit-depth=10

AV1_CFG = $(AV1_HLCQP)

VP9_COMMON  = $(CFG_GENERAL) --codec=vp9 --frame-parallel=0 --tile-columns=0 --cpu-used=0 --threads=1

VP9_HLCQP = $(VP9_COMMON) --end-usage=q --auto-alt-ref=2 #--cq-level={22,32,43,55}

VP9_CFG = $(VP9_HLCQP)


EXTRA_CFG_AV1 = "--i444 --profile=0 --bit-depth=10"
EXTRA_CFG_VP9 = "--i444 --profile=2 --bit-depth=10"

EXTRA_CFG_AV1hd = "--profile=0 --bit-depth=10"
EXTRA_CFG_VP9hd = "--profile=2 --bit-depth=10"


MAIN_SRC = /home/alex/Videos/objective-2-slow/

#########      LISTOFVIDEOS          ###########
# FOR 426 x 240
AWIDTH = 426
AHEIGHT= 240
SRC_A=$(MAIN_SRC)426x240_4:2:0_120f/
A01=$(SRC_A)bqfree_240p_120f.y4m
A02=$(SRC_A)bqhighway_240p_120f.y4m
A03=$(SRC_A)bqzoom_240p_120f.y4m
A04=$(SRC_A)chairlift_240p_120f.y4m
A05=$(SRC_A)dirtbike_240p_120f.y4m
A06=$(SRC_A)mozzoom_240p_120f.y4m

# FOR 640 x 360
BWIDTH = 640
BHEIGHT= 360
SRC_B=$(MAIN_SRC)640x360_4:2:0_120f/
B01=$(SRC_B)blue_sky_360p_120f.y4m
B02=$(SRC_B)controlled_burn_640x360_120f.y4m
B03=$(SRC_B)desktop2360p_120f.y4m
B04=$(SRC_B)kirland360p_120f.y4m
B05=$(SRC_B)mmstationary360p_120f.y4m
B06=$(SRC_B)niklas360p_120f.y4m
B07=$(SRC_B)rain2_hdr_amazon_360p_8bit.y4m
B08=$(SRC_B)red_kayak_360p_120f.y4m
B09=$(SRC_B)riverbed_360p25_120f.y4m
B10=$(SRC_B)shields2_640x360_120f.y4m
B11=$(SRC_B)snow_mnt_640x360_120f.y4m
B12=$(SRC_B)speed_bag_640x360_120f.y4m
B13=$(SRC_B)stockholm_640x360_120f.y4m
B14=$(SRC_B)tacomanarrows360p_120f.y4m
B15=$(SRC_B)thaloundeskmtg360p_120f.y4m
B16=$(SRC_B)water_hdr_amazon_360p_8bits.y4m

# FOR HD720
CWIDTH = 1280
CHEIGHT= 720
SRC_C=$(MAIN_SRC)1280x720_4:2:0_120f/
C01=$(SRC_C)boat_hdr_amazon_720p.y4m
C02=$(SRC_C)dark720p_120f.y4m
C03=$(SRC_C)FourPeople_1280x720_60_120f.y4m
C04=$(SRC_C)gipsrestat720p_120f.y4m
C05=$(SRC_C)Johnny_1280x720_60_120f.y4m
C06=$(SRC_C)KristenAndSara_1280x720_60_120f.y4m
C07=$(SRC_C)Netflix_DinnerScene_1280x720_60fps_8bit_420_120f.y4m
C08=$(SRC_C)Netflix_DrivingPOV_1280x720_60fps_8bit_420_120f.y4m
C09=$(SRC_C)Netflix_FoodMarket2_1280x720_60fps_8bit_420_120f.y4m
C10=$(SRC_C)Netflix_RollerCoaster_1280x720_60fps_8bit_420_120f.y4m
C11=$(SRC_C)Netflix_Tango_1280x720_60fps_8bit_420_120f.y4m
C12=$(SRC_C)rain_hdr_amazon_720p.y4m
C13=$(SRC_C)vidyo1_720p_60fps_120f.y4m
C14=$(SRC_C)vidyo3_720p_60fps_120f.y4m
C15=$(SRC_C)vidyo4_720p_60fps_120f.y4m

# FOR HD1080
DWIDTH = 1920
DHEIGHT= 1080
SRC_D=$(MAIN_SRC)1920x1080_4:2:0_60f/
D01=$(SRC_D)aspen_1080p_60f.y4m
D02=$(SRC_D)crowd_run_1080p50_60f.y4m
D03=$(SRC_D)ducks_take_off_1080p50_60f.y4m
D04=$(SRC_D)guitar_hdr_amazon_1080p.y4m
D05=$(SRC_D)life_1080p30_60f.y4m
D06=$(SRC_D)Netflix_Aerial_1920x1080_60fps_8bit_420_60f.y4m
D07=$(SRC_D)Netflix_Boat_1920x1080_60fps_8bit_420_60f.y4m
D08=$(SRC_D)Netflix_Crosswalk_1920x1080_60fps_8bit_420_60f.y4m
D09=$(SRC_D)Netflix_FoodMarket_1920x1080_60fps_8bit_420_60f.y4m
D10=$(SRC_D)Netflix_PierSeaside_1920x1080_60fps_8bit_420_60f.y4m
D11=$(SRC_D)Netflix_SquareAndTimelapse_1920x1080_60fps_8bit_420_60f.y4m
D12=$(SRC_D)Netflix_TunnelFlag_1920x1080_60fps_8bit_420_60f_8bit.y4m
D13=$(SRC_D)old_town_cross_1080p50_60f.y4m
D14=$(SRC_D)pan_hdr_amazon_1080p.y4m
D15=$(SRC_D)park_joy_1080p50_60f.y4m
D16=$(SRC_D)pedestrian_area_1080p25_60f.y4m
D17=$(SRC_D)rush_field_cuts_1080p_60f.y4m
D18=$(SRC_D)rush_hour_1080p25_60f.y4m
D19=$(SRC_D)seaplane_hdr_amazon_1080p_8bit.y4m
D20=$(SRC_D)station2_1080p25_60f.y4m
D21=$(SRC_D)touchdown_pass_1080p_60f.y4m

# FOR HD1080 444 or 422
EWIDTH = 1920
EHEIGHT= 1080
SRC_E=$(MAIN_SRC)1920x1080_4:4:4_4:2:0_60f/
E01=$(SRC_E)CSGO_60f.y4m
E02=$(SRC_E)DOTA2_60f_420.y4m
E03=$(SRC_E)EuroTruckSimulator2_60f.y4m
E04=$(SRC_E)Hearthstone_60f.y4m
E05=$(SRC_E)MINECRAFT_60f_420.y4m
E06=$(SRC_E)pvq_slideshow.y4m
E07=$(SRC_E)STARCRAFT_60f_420.y4m
E08=$(SRC_E)wikipedia_420.y4m

# FOR UHD4K
FWIDTH = 4096
FHEIGHT= 2160
SRC_F=$(MAIN_SRC)4096x2160_4:2:0_60f/
F01=$(SRC_F)Netflix_BarScene_4096x2160_60fps_10bit_420_60f.y4m
F02=$(SRC_F)Netflix_BoxingPractice_4096x2160_60fps_10bit_420_60f.y4m
F03=$(SRC_F)Netflix_Dancers_4096x2160_60fps_10bit_420_60f.y4m
F04=$(SRC_F)Netflix_Narrator_4096x2160_60fps_10bit_420_60f.y4m
F05=$(SRC_F)Netflix_RitualDance_4096x2160_60fps_10bit_420_60f.y4m
F06=$(SRC_F)Netflix_ToddlerFountain_4096x2160_60fps_10bit_420_60f.y4m
F07=$(SRC_F)Netflix_WindAndNature_4096x2160_60fps_10bit_420_60f.y4m
F08=$(SRC_F)street_hdr_amazon_2160p.y4m


# FOR UHD8K
GWIDTH = 8192
GHEIGHT= 4320
SRC_G=/home/alex/Videos/UHD8K/
G01=$(SRC_G)RED_Face_UHD8K_50fps_444_12bits_100f.yuv
G02=$(SRC_G)RED_Motocycle_UHD8K_48fps_444_12bits_96f.yuv

