Reuse [viewtioga](https://xeroxparcarchive.computerhistory.org/_cdcsl_93-16_/1/viewtioga/.index.html) code into a command dump_tioga

We have the sources for viewtioga, we can reuse it to dump tioga files
without the need to the Andrew Toolkit.

viewtioga is supposed to show .tioga files in unix with X, but it relies on the [Andrew Toolkit (ATK)](https://en.wikipedia.org/wiki/Andrew_Project) and building
have been proving difficult so I hacked something together to reuse the code to, for now, just print the file's text content. All .mesa files, the souruce code for the Cedar system are also tioga files as are some other text files that are not plain ASCII or the older bravo files from the Alto.

# Build
On the root of this repository run:

```bash
$ make dump_tioga
```

I will download the sources from the Xerox Archive and patch them. If you make make and a C compiler you should end up with a binary viewtioga/src/dump_tioga that you can use to look at a .mesa source code file like this:

```bash
$ wget https://xeroxparcarchive.computerhistory.org/_cdcsl_93-16_/1/cedar/release/simpleexample/ReverseName.mesa
$ ./viewtioga/src/dump_tioga ReverseName.mesa
```
