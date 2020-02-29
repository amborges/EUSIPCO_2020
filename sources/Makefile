include VIDEOINFO.mk

CPU = 0

#make dostat // to do statistical analysis
#make dotrans // to do transcoding

clean:
	rm -rf *.y4m
	rm -rf *.txt
	rm -rf log/av1encoding.log
	rm -rf log/vp9encoding.log
	rm -rf webm/*.webm
	rm -rf PNG/*.png
	rmdir webm
	rmdir PNG
	rm -rf VIDEOINFO.mk
	rm -rf Makefile
	

generateAV1:
	rm -rf encoders/BIN
	mkdir encoders/BIN
	cd encoders/BIN && cmake -DAOM_TARGET_CPU=generic -DCONFIG_AV1_HIGHBITDEPTH=1 ../aom
	cd encoders/BIN && $(MAKE) all
	cp encoders/BIN/aomenc encoders/aomenc_par_55
	cp encoders/BIN/aomdec encoders/aomdec
	
generateVP9:
	rm -rf encoders/BIN
	mkdir encoders/BIN
	cd encoders/BIN && ../libvpx/configure --target=generic-gnu --enable-vp9-highbitdepth
	cd encoders/BIN && $(MAKE)
	cp encoders/BIN/vpxenc encoders/vpxenc
	cp encoders/BIN/vpxdec encoders/vpxdec

encodeAV1:
	./../encoders/aomenc$(tp)_$(CQ) $(AV1_CFG) $(EXTRAAV1) --cq-level=$(CQ) --limit=$(FTBE) -o webm/av1$(tp)_$(CQ).webm vp9_decoded_video.y4m > log/av1$(tp)_$(CQ).log 2>&1

decodeAV1:
	./../encoders/aomdec -o av1_decoded_video.y4m $(WEBM) > phdav1.sb
	rm -rf framematrix_av1.txt
	python3 ../conversor.py $(HEIGHT)p $(CQ) av1

encodeVP9:
	./../encoders/vpxenc $(VP9_CFG) $(EXTRAVP9) --cq-level=$(CQ) --limit=$(FTBE) -o webm/vp9_$(CQ).webm vp9_decoded_video.y4m > log/vp9_$(CQ).log 2>&1

decodeVP9:
	./../encoders/vpxdec -o vp9_decoded_video.y4m $(WEBM) > phdvp9.sb
	rm -rf framematrix_vp9.txt
	python3 ../conversor.py $(HEIGHT)p $(CQ) vp9

comparing:
	gcc ../compare.c
	./a.out > log/comparacao_$(CQ).log
	rm a.out

encodingAV1:
	./../encoders/aomenc_ori $(AV1_CFG) $(EXTRAAV1) --cq-level=$(CQ) --limit=$(FTBE) -o webm/av1encoding.webm $(VIDEO) > log/av1encoding.log 2>&1
	./../encoders/aomdec -o av1_decoded_video.y4m webm/av1encoding.webm > phdav1.sb
	rm -rf framematrix_av1.txt
	python3 ../conversor.py $(HEIGHT)p $(CQ) av1

encodingVP9:
	./../encoders/vpxenc $(VP9_CFG) $(EXTRAVP9) --cq-level=$(CQ) --limit=$(FTBE) -o webm/vp9encoding.webm $(VIDEO) > log/vp9encoding.log 2>&1
	./../encoders/vpxdec -o vp9_decoded_video.y4m webm/vp9encoding.webm > phdvp9.sb
	rm -rf framematrix_vp9.txt
	python3 ../conversor.py $(HEIGHT)p $(CQ) vp9

encoding:
	./../encoders/vpxenc $(VP9_CFG) $(EXTRAVP9) --cq-level=20 --limit=$(FTBE) -o webm/vp9encoding.webm $(VIDEO) > log/vp9encoding.log 2>&1
	./../encoders/vpxdec -o vp9_decoded_video.y4m webm/vp9encoding.webm > phdvp9.sb
	rm -rf framematrix_vp9.txt
	python3 ../conversor.py $(HEIGHT)p 20 vp9

transcode:
	@rm -rf $(folder)
	@mkdir $(folder)
	@mkdir $(folder)/log
	@mkdir $(folder)/webm
	@mkdir $(folder)/PNG
	@cp Makefile $(folder)/Makefile
	@cp VIDEOINFO.mk $(folder)/VIDEOINFO.mk
	@taskset -c $(CPU) $(MAKE) -C $(folder) encoding
	@for cq in $(CQs) ; do \
		taskset -c $(CPU) $(MAKE) -C $(folder) encodeVP9 CQ=$$cq ; \
		taskset -c $(CPU) $(MAKE) -C $(folder) encodeAV1 tp=_ori CQ=$$cq ; \
		taskset -c $(CPU) $(MAKE) -C $(folder) encodeAV1 tp=_fst CQ=$$cq ; \
	done
	$(MAKE) -C $(folder) clean

statistic:
	@rm -rf $(folder)
	@mkdir $(folder)
	@mkdir $(folder)/log
	@mkdir $(folder)/webm
	@mkdir $(folder)/PNG
	@cp Makefile $(folder)/Makefile
	@cp VIDEOINFO.mk $(folder)/VIDEOINFO.mk
	@for cq in $(CQs) ; do \
		taskset -c $(CPU) $(MAKE) -C $(folder) encodingVP9 CQ=$$cq ; \
		taskset -c $(CPU) $(MAKE) -C $(folder) encodingAV1 CQ=$$cq ; \
		$(MAKE) -C $(folder) comparing CQ=$$cq ; \
	done
	$(MAKE) -C $(folder) clean
	
	
do_statistic:
	$(MAKE) statistic folder=$(notdir $(VIDEO))_s

do_transcode:
	$(MAKE) transcode folder=$(notdir $(VIDEO))_t


##########################################
##########################################
# ESTATISTICAS
##########################################
##########################################

stat240:
	@$(MAKE) do_statistic VIDEO=$(A02) WIDTH=$(AWIDTH) HEIGHT=$(AHEIGHT) CPU=$(CPU)
	@$(MAKE) do_statistic VIDEO=$(A05) WIDTH=$(AWIDTH) HEIGHT=$(AHEIGHT) CPU=$(CPU)

stat360:
	@$(MAKE) do_statistic VIDEO=$(B01) WIDTH=$(BWIDTH) HEIGHT=$(BHEIGHT) CPU=$(CPU)
	@$(MAKE) do_statistic VIDEO=$(B16) WIDTH=$(BWIDTH) HEIGHT=$(BHEIGHT) CPU=$(CPU)

stat720:
	@$(MAKE) do_statistic VIDEO=$(C07) WIDTH=$(CWIDTH) HEIGHT=$(CHEIGHT) CPU=$(CPU)
	@$(MAKE) do_statistic VIDEO=$(C09) WIDTH=$(CWIDTH) HEIGHT=$(CHEIGHT) CPU=$(CPU)

stat1080_chato:
	@$(MAKE) do_statistic VIDEO=$(D04) WIDTH=$(DWIDTH) HEIGHT=$(DHEIGHT) CPU=$(CPU)
	
stat1080:
	@$(MAKE) do_statistic VIDEO=$(D12) WIDTH=$(DWIDTH) HEIGHT=$(DHEIGHT) CPU=$(CPU)

stat1080444:
	@$(MAKE) do_statistic VIDEO=$(E01) WIDTH=$(EWIDTH) HEIGHT=$(EHEIGHT) CPU=$(CPU)
	@$(MAKE) do_statistic VIDEO=$(E06) WIDTH=$(EWIDTH) HEIGHT=$(EHEIGHT) CPU=$(CPU)

stat4k:
	@$(MAKE) do_statistic VIDEO=$(F01) WIDTH=$(FWIDTH) HEIGHT=$(FHEIGHT) CPU=$(CPU)
	@$(MAKE) do_statistic VIDEO=$(F08) WIDTH=$(FWIDTH) HEIGHT=$(FHEIGHT) CPU=$(CPU)





##########################################
##########################################
# TRANSCODIFICACOES
##########################################
##########################################

trans240:
	@$(MAKE) do_transcode VIDEO=$(A01) WIDTH=$(AWIDTH) HEIGHT=$(AHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(A03) WIDTH=$(AWIDTH) HEIGHT=$(AHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(A04) WIDTH=$(AWIDTH) HEIGHT=$(AHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(A06) WIDTH=$(AWIDTH) HEIGHT=$(AHEIGHT) CPU=$(CPU)

trans360:
	@$(MAKE) do_transcode VIDEO=$(B07) WIDTH=$(BWIDTH) HEIGHT=$(BHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(B08) WIDTH=$(BWIDTH) HEIGHT=$(BHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(B11) WIDTH=$(BWIDTH) HEIGHT=$(BHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(B14) WIDTH=$(BWIDTH) HEIGHT=$(BHEIGHT) CPU=$(CPU)

trans720:
	@$(MAKE) do_transcode VIDEO=$(C02) WIDTH=$(CWIDTH) HEIGHT=$(CHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(C05) WIDTH=$(CWIDTH) HEIGHT=$(CHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(C08) WIDTH=$(CWIDTH) HEIGHT=$(CHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(C10) WIDTH=$(CWIDTH) HEIGHT=$(CHEIGHT) CPU=$(CPU)

trans1080:
	@$(MAKE) do_transcode VIDEO=$(D02) WIDTH=$(DWIDTH) HEIGHT=$(DHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(D08) WIDTH=$(DWIDTH) HEIGHT=$(DHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(D15) WIDTH=$(DWIDTH) HEIGHT=$(DHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(D19) WIDTH=$(DWIDTH) HEIGHT=$(DHEIGHT) CPU=$(CPU)

trans1080444:
	@$(MAKE) do_transcode VIDEO=$(E02) WIDTH=$(EWIDTH) HEIGHT=$(EHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(E05) WIDTH=$(EWIDTH) HEIGHT=$(EHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(E07) WIDTH=$(EWIDTH) HEIGHT=$(EHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(E08) WIDTH=$(EWIDTH) HEIGHT=$(EHEIGHT) CPU=$(CPU)


trans4k:
	@$(MAKE) do_transcode VIDEO=$(F03) WIDTH=$(FWIDTH) HEIGHT=$(FHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(F04) WIDTH=$(FWIDTH) HEIGHT=$(FHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(F06) WIDTH=$(FWIDTH) HEIGHT=$(FHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(F07) WIDTH=$(FWIDTH) HEIGHT=$(FHEIGHT) CPU=$(CPU)

trans8k:
	@$(MAKE) do_transcode VIDEO=$(G01) WIDTH=$(GWIDTH) HEIGHT=$(GHEIGHT) CPU=$(CPU)
	@$(MAKE) do_transcode VIDEO=$(G02) WIDTH=$(GWIDTH) HEIGHT=$(GHEIGHT) CPU=$(CPU)

dostat:
	$(MAKE) stat240
	$(MAKE) stat360
	$(MAKE) stat720
	$(MAKE) stat1080
	$(MAKE) stat1080_chato EXTRAAV1=$(EXTRA_CFG_AV1hd) EXTRAVP9=$(EXTRA_CFG_VP9hd)
	$(MAKE) stat1080444 EXTRAAV1=$(EXTRA_CFG_AV1) EXTRAVP9=$(EXTRA_CFG_VP9)
	$(MAKE) stat4k EXTRAAV1=$(EXTRA_CFG_AV1) EXTRAVP9=$(EXTRA_CFG_VP9)

dotrans:
	$(MAKE) trans240
	$(MAKE) trans360
	$(MAKE) trans720
	$(MAKE) trans1080
	$(MAKE) trans1080444 HB=hb EXTRAAV1=$(EXTRA_CFG_AV1) EXTRAVP9=$(EXTRA_CFG_VP9)
	$(MAKE) trans4k HB=hb EXTRAAV1=$(EXTRA_CFG_AV1) EXTRAVP9=$(EXTRA_CFG_VP9)
	$(MAKE) trans8k HB=hb EXTRAAV1=$(EXTRA_CFG_AV1) EXTRAVP9=$(EXTRA_CFG_VP9)

