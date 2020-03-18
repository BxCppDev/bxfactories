# Quick start for developpers

## Prerequisites

Make sure the following software are installed and setup on your system:

- cmake
- C++ compiler with C++11 support
- make
- Boost library (headers only)

## Build/installation

Cd in the BxFactories source directory then run:
```sh
$ ./tools/build.sh
```

The build is done in the ``_build.d/`` directory. The installation
is in ``_install.d/``.


## Clean

Cd in the BxFactories source directory then run:
```sh
$ ./tools/clean.sh
```
Both ``_build.d/`` and ``_install.d/`` directories are removed.
