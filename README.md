# Cedar (experiments and) tools

![Cedar tree in a japanese garden. Image credit to free-images.com](./misc/cedar_japanese_cedar_tree.jpg)

I got obsessed with the ideas of Alan Kay's mythical [Dynabook](https://en.wikipedia.org/wiki/Dynabook) and it's first incarnation, the [Alto](https://en.wikipedia.org/wiki/Xerox_Alto), years ago. That led me to explore without much depth of Smalltalk descendants, [Squeak](https://squeak.org/) and [Pharo](https://pharo.org/) and it lead knower. Last time my interest peaked again I went to look if anybody had done a FPGA implementation of the Alto, feels like it would be a bit of a challenge but maybe not as much as I believe, I found nothing. Not being able to work with hardware right now I got back to study Xerox PARC history and fix/translates some related wikipedia articles to the Portuguese language wikipedia. I then got interested in the other languages they used at the PARC, like [BCPL](https://en.wikipedia.org/wiki/BCPL) and [Mesa](https://en.wikipedia.org/wiki/Mesa_\(programming_language\)) and it's descendants [Cedar](https://en.wikipedia.org/wiki/Cedar_\(programming language\)) (name of the language and the environment).

## Before anything... Emulators!

You shoud go try some full system emulators, most are courtesy of the [Living Computer Museum](https://en.wikipedia.org/wiki/Living_Computers:_Museum_%2B_Labs) R.I.P.

- Some information about the Alto and a emulator that runs inside the browser http://retroweb.maclab.org/articles/Xerox-Alto.html
- ContraAltoJS: port of the next Alto emulator to run inside the browser https://archives.loomcom.com/contraltojs/
- ContraAlto: an Alto emulator for dotnet and mono (like for Linux and other OSs that run Mono) https://github.com/livingcomputermuseum/ContrAlto
- Darkstar: an Xerox Dandelion workstation emulator for dotnet and mono (same as the above) https://github.com/livingcomputermuseum/Darkstar


## Research
I looked if any Mesa/Cedar compilers were available for modern systems but they all stuck on the Alto and it's D line of computers as far as I know. Frustratingly enough I never managed to compile anything on those machines.

I then looked if there was any BCPL compiler for Linux. The BCPL creator still mantes a compiler distribution at [https://www.cl.cam.ac.uk/~mr10/BCPL.html]() and it works, but only to generate intermediate code and run it, the binary compiler seems to be broken. I imagined I would find a version of the Mesa compiler written in BCPL and somehow get it to build and run on a modern computer but if there's on I haven't found it yet.

This is a BCPL reference manual from Xerox http://www.bitsavers.org/pdf/xerox/alto/bcpl/BCPL_Reference_Manual_Sep75.pdf

What I later found when looking for the C source code of one of the Cedar ports to Unix, was binaries for Sun Microsystems SunOS (the OS before Solaris) for the architecture sun4 like https://xeroxparcarchive.computerhistory.org/_cdcsl_93-16_/1/cedar/cdpackage/solaris2/.index.html. After looking for the particular version of SunOS those binaries were compiled for SunOS 2.6 (aka Solaris 5.6) to try to increase my chances of success (and a archive.org hack/outage later) I found the installed ISO file **[TODO: link the internet achieve here]**. I did familiarized myself with SunOS and running it on QEmu using the _qemu-system-sparc_ emulator with (and I figured this out later when I got the Cedar environment to run) a particular flag **[TODO: document the flag here and add the full command line here]**. Turns out those binary files were part of a mostly functional version of Cedar. Besides telling QEmu to add a float point extension to the emulated Sparc CPU and after lot's of trial en error it was only necessary to rename the folders in https://xeroxparcarchive.computerhistory.org/_cdcsl_93-16_/1/cedar/release/.index.html since the capitalization of the names was lost when I downloaded them. **[TODO: find a way to keep the correct case or write a script to rename the folders correctly]**. You should be able to run the environment by replacing the custom X server that SunOS defaults to but I didn't try that and ran a Xephir nested X server on another machine.

**[TODO: try to find this update mentioned at wikipedia "Last update was Solaris 2.6 5/98."]**

![Cedar enviroment screenshot](misc/cedar_environment.png)

I haven't managed to compile any Mesa/Cedar code using the Mimosa compiler yet.

![My attempt at writing a "Hello, World!" program in Mesa](misc/hello_world_attempt.png)

I'll include instructions on how I manage to run a version of the Cedar system on SunOS from https://xeroxparcarchive.computerhistory.org/_cdcsl_93-16_/1/cedar/.index.html in the future.


## Other links

- Ken Shirriff's [Simulating a Xerox Alto with the ContrAlto simulator: games and Smalltalk](http://www.righto.com/2016/10/simulating-xerox-alto-with-contralto.html)
- Ken did manage to compile a hello world BCPL program on the Alto (emulator): ["Hello world" in the BCPL language on the Xerox Alto simulator](http://www.righto.com/2016/06/hello-world-in-bcpl-language-on-xerox.html)
- End of course the Alto Restoration playlist https://www.youtube.com/playlist?list=PL-_93BVApb58I3ZV67LW3S_JEMFnDrQDj where CuriousMark, Ken Shirriff and Carl Claunch fix Alan Kay's Alto, if I'm not mistaken.


## Goal of this repo

This repository will holds some incomplete tools and experiments with code and fonts from Xerox PARC filesystem archive https://xeroxparcarchive.computerhistory.org/Xerox_PARC_source_code.html and documents my work.


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

## Some notes

- The "!2" in the end of the font files is an artefact of the versioning system used by the archives, the two means it is the second version of that particular file.
- There's a lot of documentation in the archives and at bitsavers.org and probably copies of the Cedar environment source code out there besides the one at the Computer History Museum, like this one at the internet archive https://archive.org/details/bitsavers_xeroxparcteriencesCreatingaPortableCedar_1335085/page/n3/mode/2up they just need to be studied systematically.
- I don't guarantee all information here is precise and correct and I welcome corrections.
