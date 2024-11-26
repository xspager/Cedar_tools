# Cedar (experiments and) tools

Some incomplete tools and experiments with code and fonts from Xerox PARC filesystem archive [https://xeroxparcarchive.computerhistory.org/Xerox_PARC_source_code.html]()

I'll include intructions on how to run the Cedar sytem on SunOS from [https://xeroxparcarchive.computerhistory.org/_cdcsl_93-16_/1/cedar/.index.html]() in the future.


## Bravo fonts and the AL font format

You can downloads the fonts used by the [Bravo](https://en.wikipedia.org/wiki/Bravo_(editor)) editor from [https://xeroxparcarchive.computerhistory.org/indigo/bravox/fonts/.index.html]() by calling:

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


## Some notes

The "!2" in the end of the font files is an artefact of the versioning system used by the archives, the two means it is the second version of that particular file.
