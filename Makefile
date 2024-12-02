ALL=print_al_font

CFLAGS+= -Wall -Wpedantic -ggdb

print_al_font:	print_al_font.c
	$(CC) $(CFLAGS) -o print_al_font print_al_font.c

download_bravox_fonts:
	mkdir -p Fonts
	cd Fonts && wget -c -r -nd -R "*.html*" -r "*.pdf*" https://xeroxparcarchive.computerhistory.org/indigo/bravox/fonts/.index.html; echo $?

download_altofonts:
	mkdir -p Fonts
	cd Fonts && wget -c -r -nd -R "*.html*" -r "*.pdf*" https://xeroxparcarchive.computerhistory.org/indigo/altofonts/original/.index.html; echo $?

download_altofonts_eighbits:
	mkdir -p Fonts
	cd Fonts && wget -c -r -nd -R "*.html*" -r "*.pdf*" https://xeroxparcarchive.computerhistory.org/indigo/altofonts/eightbit/.index.html; echo $?

download_smalltalk_76:
	mkdir -p Smalltalk-76
	cd Smalltalk-76 && wget -c -r -nd -R "*.html*" -r "*.pdf*" https://xeroxalto.computerhistory.org/Filene/Smalltalk-76/.index.html; echo $?


.PHONY: clean

clean:
	rm -f $(ALL)
