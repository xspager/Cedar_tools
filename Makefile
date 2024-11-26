ALL=print_al_font

CFLAGS+= -Wall -Wpedantic

print_al_font:	print_al_font.c
	$(CC) $(CFLAGS) -o print_al_font print_al_font.c

download_fonts:	downloaded_fonts
	mkdir -p Fonts
	cd Fonts && wget -c -r -nd -R "*.html*" -r "*.pdf*" https://xeroxparcarchive.computerhistory.org/indigo/bravox/fonts/.index.html; echo $?

.PHONY: clean

clean:
	rm -f $(ALL)
