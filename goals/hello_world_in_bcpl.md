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
