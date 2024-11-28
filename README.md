# Cedar (experiments and) tools

![Cedar tree in a japanese garden. Image credit to free-images.com](./misc/cedar_japanese_cedar_tree.jpg)

I got obsessed with the ideas of Alan Kay's mythical [Dynabook](https://en.wikipedia.org/wiki/Dynabook) and it's first incarnation, the [Alto](https://en.wikipedia.org/wiki/Xerox_Alto), years ago. That led me to explore without much depth of Smalltalk descendants, [Squeak](https://squeak.org/) and [Pharo](https://pharo.org/) and it lead knower. Last time my interest peaked again I went to look if anybody had done a FPGA implementation of the Alto, feels like it would be a bit of a challenge but maybe not as much as I believe, I found nothing. Not being able to work with hardware right now I got back to study Xerox PARC history and fix/translates some related wikipedia articles to the Portuguese language wikipedia. I then got interested in the other languages they used at the PARC, like [BCPL](https://en.wikipedia.org/wiki/BCPL) and [Mesa](https://en.wikipedia.org/wiki/Mesa_\(programming_language\)) and it's descendants [Cedar](https://en.wikipedia.org/wiki/Cedar_\(programming language\)) (name of the language and the environment).

## Before anything... Emulators!

You shoud go try some full system emulators, most are courtesy of the [Living Computer Museum](https://en.wikipedia.org/wiki/Living_Computers:_Museum_%2B_Labs) R.I.P.

- Some information about the Alto and a emulator that runs inside the browser http://retroweb.maclab.org/articles/Xerox-Alto.html
- ContraAltoJS: port of the next Alto emulator to run inside the browser https://archives.loomcom.com/contraltojs/
- ContraAlto: an Alto emulator for dotnet and mono (like for Linux and other OSs that run Mono) https://github.com/livingcomputermuseum/ContrAlto
- Darkstar: an Xerox Dandelion workstation emulator for dotnet and mono (same as the above) https://github.com/livingcomputermuseum/Darkstar
- Dwarf, a Mesa machine architecture emulator for Xerox 6085 and Guam workstations https://github.com/devhawala/dwarf


## Research
I looked if any Mesa/Cedar compilers were available for modern systems but they all stuck on the Alto and it's D line of computers as far as I know. Frustratingly enough I never managed to compile anything on those machines.

I then looked if there was any BCPL compiler for Linux. The BCPL creator still mantes a compiler distribution at [https://www.cl.cam.ac.uk/~mr10/BCPL.html]() and it works, but only to generate intermediate code and run it, the binary compiler seems to be broken. I imagined I would find a version of the Mesa compiler written in BCPL and somehow get it to build and run on a modern computer but if there's on I haven't found it yet.

This is a BCPL reference manual from Xerox http://www.bitsavers.org/pdf/xerox/alto/bcpl/BCPL_Reference_Manual_Sep75.pdf

What I later found when looking for the C source code of one of the Cedar ports to Unix, was binaries for Sun Microsystems SunOS (the OS before Solaris) for the architecture sun4 like https://xeroxparcarchive.computerhistory.org/_cdcsl_93-16_/1/cedar/cdpackage/solaris2/.index.html. After looking for the particular version of SunOS those binaries were compiled for, SunOS 2.6 (aka Solaris 5.6), to try to increase my chances of success (and a archive.org hack/outage later) I found the installed ISO file **[TODO: link the internet achieve here]**. I did familiarized myself with SunOS and running it on QEmu using the _qemu-system-sparc_ emulator with (and I figured this out later when I got the Cedar environment to run) a particular flag *-cpu 'TI-SuperSparc-II,+float128'* (note we are adding *+float128* to what would be the default value for the -cpu flag). Turns out those binary files were part of a mostly functional version of Cedar. Besides telling QEmu to add a float point extension to the emulated Sparc CPU and after lot's of trial en error it was only necessary to rename the folders in https://xeroxparcarchive.computerhistory.org/_cdcsl_93-16_/1/cedar/release/.index.html since the capitalization of the names was lost when I downloaded them. You should be able to run the environment by replacing the custom X server that SunOS defaults to but I didn't try that and ran a Xephir nested X server on another machine.

**[TODO: try to find this update mentioned at wikipedia "Last update was Solaris 2.6 5/98."]**

![Cedar enviroment screenshot](misc/cedar_environment.png)

I haven't managed to compile any Mesa/Cedar code using the Mimosa compiler yet.

![My attempt at writing a "Hello, World!" program in Mesa](misc/hello_world_attempt.png)


## How to build yourself a Sun (OS install)
Since most of us don't own a working (or not) SPARCstation you'll need to install a version of QEmu built with the SPARC emulator or install the package for it, something like *qemu-system-sparc* and also *qemu-utils* (to create an empty disk image). On a Debian based system should be enough to:

```bash
$ sudo apt install qemu-system-sparc qemu-utils
```

Download the install ISO from https://fsck.technology/software/Sun%20Microsystems/Solaris%20Install%20Media/Sun%20Solaris%202.6/Sun%20Solaris%202.6%2005-1998/ (this one marches the SHA1 hash of the ISO I believe I used)

**TODO: Check the other ISO files from the same site, and also check the internet archive images**

For now we will follow a guide I used. You can ignore the instructions about compiling QEmu and using the ROM file since you should be able to just use what QEmu provides, but this guide cover most of what you need to do https://astr0baby.wordpress.com/2018/09/22/running-solaris-2-6-sparc-on-qemu-system-sparc-in-linux-x86_64-mint-19/

> [!WARNING]
> The details about disk size, disk formatting and partition matter.

**TODO: Maybe document how to label (something SunOS requires) the disk on Linux as expected by SunOS before the installation if that was really possible to save one reboot. Add the how to mount a qcow2 image.**

> [!TIP]
> To be able to run the binaries I found and therefore run the Cedar system you must pass the flag *-cpu 'TI-SuperSparc-II,+float128'* when you call QEmu.

```bash
$ qemu-img create -f qcow2 sunos_26.qcow2 4G
$ qemu-system-sparc -M SS-20 -cpu 'TI-SuperSparc-II,+float128' -net nic,macaddr=00:03:BA:04:A5:AD -net user -m slots=4,maxmem=256M -cdrom solaris_2.6_598_sparc.iso -M SS-20 -vga cg3 -smp 2 -hda sunos_26.qcow2
```

### Bonus
In case you want to play around with [Forth](https://en.wikipedia.org/wiki/Forth_\(programming_language\)) you can boot one of the SPARCstation ROMS https://github.com/andarazoroflove/sparc and have a go with the *ok* prompt:

```bash
$ qemu-system-sparc -bios ss5.bin -M SS-5
```

You need to press the key Stop-A, and since we don't have a Sun keyboard, go on the menu View > compatmonitor0 or just press CTRL+ALT+2 and type *sendkey stop-a* then menu View > sun-tcg or CTRL+ALT+1 to go back. Try typing this at the *ok* prompt:

```forth
5 2 + 10 * .
```

Of course QEmu's included version of OpenFirmware is also a Forth prompt without the *ok* part so we can so that same without the proprietary firmware.

## Getting very own Cedar
Now that we have a "machine" running SunOS 2.6 we better backup the disk image by coping sunos_26.qcow2 to something like sunos_26.qcow2.back before we proceed. And then...

I'll later include instructions on how I manage to run a version of the Cedar system on SunOS from https://xeroxparcarchive.computerhistory.org/_cdcsl_93-16_/1/cedar/.index.html in the future. Maybe a script to run on SunOS (maybe there are no way to download anything, maybe you could install Netscape :raised_eyebrow: ...it does have that browser written in Java out of the box though)

Basically a recursive wget from the URL above but we can do a better than that, since we might need to rename the folders anyway we might as well put the download as part of a script. Check the Makefile to see the flags I'm using to download fonts.

> [!CAUTION]
> You need to set the environment variable **TODO: check if it's XEROXCEDAR or something**

**[TODO: find a way to keep the correct case or write a script to rename the folders correctly]**


## Other links

- Ken Shirriff's [Simulating a Xerox Alto with the ContrAlto simulator: games and Smalltalk](http://www.righto.com/2016/10/simulating-xerox-alto-with-contralto.html)
- Ken did manage to compile a hello world BCPL program on the Alto (emulator): ["Hello world" in the BCPL language on the Xerox Alto simulator](http://www.righto.com/2016/06/hello-world-in-bcpl-language-on-xerox.html)
- End of course the Alto Restoration playlist https://www.youtube.com/playlist?list=PL-_93BVApb58I3ZV67LW3S_JEMFnDrQDj where CuriousMark, Ken Shirriff and Carl Claunch fix Alan Kay's Alto, if I'm not mistaken.


## Goal of this repo

This repository will holds some incomplete tools and experiments with code and fonts from Xerox PARC filesystem archive https://xeroxparcarchive.computerhistory.org/Xerox_PARC_source_code.html and documents my work.


## My goals
- [x] [Hello world in BCLP on the Alto](#hello-world-in-bcpl)
- [ ] Hello world in Mesa
- [ ] Hello world in Cedar
- [ ] HTTP server maybe? :raised_eyebrow:
- [ ] Get Mimosa to generate C code that compiles and run Linux :exploding_head:
- [ ] Compile Cedar for Linux :exploding_head:

## Goal details

### Hello world in BCPL

Thanks to http://www.righto.com/2016/06/hello-world-in-bcpl-language-on-xerox.html

```c
// Hello, World!
get "streams.d"
external
[
  Ws
]

let Main() be
[
  Ws("Hello, World!*N")
]
```

![Hello world in BCPL running on the ContrAltoJS](misc/hello_bcpl.png)


## Bravo fonts and the AL font format

This PDF describes various font formats used by the Alto http://www.bitsavers.org/pdf/xerox/alto/printing/AltoFontFormats_Oct1980.pdf

You can downloads the fonts used by the [Bravo](https://en.wikipedia.org/wiki/Bravo_\(editor\)) editor from https://xeroxparcarchive.computerhistory.org/indigo/bravox/fonts/.index.html by calling:

```bash
$ make download_fonts
```

Then use for example:

```bash
$ python3 read_al_font.py 'Fonts/Arrows10.AL!2'
```

To see an "ASCII" art representation of the font.

You can also make and run print\_al\_font.c to see some information about a font:

```bash
$ make
$ ./print_al_font Fonts/Splunk14.AL\!2
```
**[TODO: Add my ImHex patter files that I attempted to put toguether to explore the fonts]**

> [!NOTE]
> The "!2" in the end of the font files is an artefact of the versioning system used by the archives, the two means it is the second version of that particular file.

> [!NOTE]
> There's a lot of documentation in the archives and at bitsavers.org and probably copies of the Cedar environment source code out there besides the one at the Computer History Museum, like this one at the internet archive https://archive.org/details/bitsavers_xeroxparcteriencesCreatingaPortableCedar_1335085/page/n3/mode/2up they just need to be studied systematically.

> [!CAUTION]
> I don't guarantee all information here is precise and correct and I welcome corrections.
