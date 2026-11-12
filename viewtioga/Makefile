CFLAGS = -std=c89 -ggdb -O0 -Wall
#WASM_CFLAGS=\
#	--target=wasm32 \
#	-nostdlib \
#	-Wl,--no-entry \
#	-Wl,--export=bar \
#	-Wl,--lto-O3 \
#	-Wl,--allow-undefined \
#	-g \
#
all:	main dump_tioga

tread.o:	tread.h tread.c
	$(CC) $(CFLAGS) -c -o tread.o tread.c

tioga.o:	tioga.h tioga.c
	$(CC) $(CFLAGS) -c -o tioga.o tioga.c

main:	main.c tread.o tioga.o
	$(CC) $(CFLAGS) -o main main.c tioga.o tread.o

dump_tioga:	main.c tread.o tioga.o
	$(CC) $(CFLAGS) -DDUMP -o dump_tioga main.c tioga.o tread.o

#tread.wasm: tread.h tread.c
#	clang $(WASM_CFLAGS) -DWASM -o tread.wasm tread.c

clean:
	$(RM) tread.o tioga.o

remove_downloaded:
	$(RM) -r README bin/ lib/ man/ src/ src_new/ top/
