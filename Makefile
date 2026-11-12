ALL=print_al_font

CFLAGS+= -Wall -Wpedantic -ggdb -std=c99

XEROX_PARC_ARCHIVE_URL = "https://xeroxparcarchive.computerhistory.org"

print_al_font:	print_al_font.c
	$(CC) $(CFLAGS) -o print_al_font print_al_font.c

download_bravox_fonts:
	mkdir -p Fonts
	cd Fonts && wget -c -r -nd -R "*.html*" -r "*.pdf*" https://xeroxparcarchive.computerhistory.org/indigo/bravox/fonts/.index.html; echo $?

download_altofonts_original:
	mkdir -p Fonts
	cd Fonts && wget -c -r -nd -R "*.html*" -r "*.pdf*" https://xeroxparcarchive.computerhistory.org/indigo/altofonts/original/.index.html; echo $?

download_altofonts_eighbits:
	mkdir -p Fonts
	cd Fonts && wget -c -r -nd -R "*.html*" -r "*.pdf*" https://xeroxparcarchive.computerhistory.org/indigo/altofonts/eightbit/.index.html; echo $?

download_smalltalk_76:
	mkdir -p Smalltalk-76
	cd Smalltalk-76 && wget -c -r -nd -R "*.html*" -r "*.pdf*" https://xeroxalto.computerhistory.org/Filene/Smalltalk-76/.index.html; echo $?

download_cedar_iso:
	wget -c https://softwarepreservation.computerhistory.org/mesa/src/cedar/DISC_IMAGE_NUMBER_1.iso

download_viewtioga:
	# mkdir -p viewtioga
	cd viewtioga && wget -c \
		-nH '# no hostname on the destin path for the downloaded files' \
		--cut-dirs=3 '# skip the deep path  /_cdcsl_93-16_/1/viewtioga/' \
		-r '# recursive' \
		-R "*.html*" -R "*.pdf*" -R "*.o*" '# reject the files matching' \
	   	$(XEROX_PARC_ARCHIVE_URL)/_cdcsl_93-16_/1/viewtioga/.index.html; echo $?
download_bonsai2.0:
	mkdir -p bonsai2.0
	cd bonsai2.0 && wget -c \
		-nH '# no hostname on the destin path for the downloaded files' \
		--cut-dirs=3 '# skip the deep path  /_cdmiwok_01_/bonsai2.0/pcr/' \
		-r '# recursive' \
		-R "*.html*" -R "*.pdf*" -R "*.o*"  '# reject the files matching' \
	   	$(XEROX_PARC_ARCHIVE_URL)/_cdmiwok_01_/bonsai2.0/pcr/.index.html; echo $?

build_view_tioga:	download_viewtioga
	cd viewtioga && mv src/Makefile src/Makefile.orig
	cd viewtioga && patch --directory=src/ < my.patch
	cd viewtioga && ln -fs ../main.c src/main.c
	cd viewtioga && ln -fs ../Makefile src/Makefile
	cd viewtioga/src && make


.PHONY: clean

clean:
	rm -f $(ALL)
