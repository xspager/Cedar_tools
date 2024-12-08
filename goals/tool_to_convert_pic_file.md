# Tool to convert PIC files to something modern
A PIC file is a bitmap image file with a simple header. You can use the ImHex pattern file [../ImHex_pattern_files/pic.hexpat](ImHex_pattern_files/pic.hexpat) to see the simple header structure of a PIC file, it has three words(2 bytes) in big endian encoding for width, height and an extra word that I haven't figured out the meaning, and the rest of bytes in the file are.

[Gimp](https://www.gimp.org/) was pretty useful after I discovered it open files of the type "Raw image data", once you click in open you have to select Image type as "B&W 1 bit", set the offset to 6 (to skip the header) and if you know the width of the image by openning with ImHex set the width, rounded you to a multiple of 16 because the bits/pixels are stored in full words of 16 bits, or guess sliding the value until the image makes sense. You will want to use the menu Colors > Invert. Note that after inverting the colors you might get a black bar on the right, that's na artifact of the bits being stored as 16 bits words. This same trick can be used to have a look at the .al fonts too.

Next I considered writing a script to automate Gimp or a Python script to use Pillow, but I remembered about the old image file formats PBM I played with a long time ago https://en.wikipedia.org/wiki/Netpbm#File_formats, the .pbm file is very similar to PIC, the difference is that it has a ascii header instead, so I wrote [pic_to_pbm.py](../pic_to_pbm.py) to replace the header and now we have something that can be open by a lot of programs and you can convert using the programs from [Netpbm](https://netpbm.sourceforge.net/doc/pbm.html), to be exact *pam_to_png* to convert it PNG. The .pbm file should be viwable with many software but nothing beats PNG for widespread support.

```bash
$ python3 ../pic_to_pnm.py elf.pic\!1 elf.pbm
$ pamtopng elf.pbm > elf.png
```

![elf.pic from https://xeroxalto.computerhistory.org/Filene/Smalltalk-76/.index.html](../misc/elf.png)
