# Contributing to https://github.com/alex-courtis/wideriver

Thank you for contribution!

Ideas, bug fixes and enhancements are always welcome.

Please raise an [issue](https://github.com/alex-courtis/wideriver/issues), fork the repository and raise a [PR](https://github.com/alex-courtis/wideriver/pulls).

## Dependencies

### Build
* GNU make
* gcc or clang
* wayland
* wayland-protocols
* wlroots

### Test
* valgrind
* [cmocka](https://cmocka.org/)
* [include-what-you-use](https://include-what-you-use.org/)

### Doc
* [pandoc](https://pandoc.org)
* [markdown-toc](https://github.com/jonschlinkert/markdown-toc)

Most will be available if you are running a wlroots based compositor like sway.

## Development

gcc is the default for packaging reasons, however clang is preferred.

Set CC and CXX when invoking make:

`make CC=clang CXX=clang++ ...`

[ccls](https://github.com/MaskRay/ccls) using clang is configured via `.ccls`, for editors that support the [Language Server Protocol](https://microsoft.github.io/language-server-protocol/).

### Compile

`make`

### Test

`make test`

cmocka is used for unit testing. Individual tests with `--wrap` definitions are defined in `tst/GNUmakefile`.

Please add tests when defining new functionality.

Individual tests may be run via test-name e.g.
`make test-cfg`

Valgrind test by appending `-vg` e.g.
`make test-vg`
`make test-cfg-vg`

### Lint

`make cppcheck`

Please resolve all issues before committing.

### Check Includes

include-what-you-use is configured to run for `src` and `tst`.

`make iwyu`

Necessary changes will be indicated in the output with "should".

See all violations:

`make -k iwyu > /dev/null`

## Documentation

Please ensure that documentation is updated when adding new features or changing CLI arguments, including defaults.

pandoc is used to generate readme and man page from templates in `doc/templ/*md`

It's sometimes easier to use the pandoc docker image than to install it:
```
#!/bin/sh
docker run --rm --volume "${PWD}:/data" --volume "/tmp:/tmp" --user "$(id -u):$(id -g)" pandoc/latex "$@"
```

markdown-toc is used to inject the table of contents into `README.md`

Update the templates and run `make doc`.

Preview the man page via `man -l man/wideriver.1`

Commit `README.md` and `man/wideriver.1`

## Style

Please match the style of the surrounding code and obey `.editorconfig`. Default vim C-indenting `gg=G` is preferred.

