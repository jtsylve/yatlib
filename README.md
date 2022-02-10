# YATLib [![Build Status](https://app.travis-ci.com/jtsylve/yatlib.svg?branch=master)](https://app.travis-ci.com/github/jtsylve/yatlib)
Yet Another Template Library for C++

The goals of this project are to provide a centralized library to hold the various general-purpose types that I've used across my projects as well as to provide implementations of library features from C++20 and beyond as well as a select few proposals that have not yet made it into standards.

When standard implementations are available, those in the `yat` namespace will fall back to aliases of the standard types.  

Documentation on current library features can be found [here](include/yatlib/README.md).

NOTE: This is currently a hobby project started during my COVID-19 isolation and thus no guarantees are given as to which features will be implemented.  I'm currently adding features as I need them in other projects.  If you would like me to support a feature, please let me know by filing an issue or submitting a pull request.

## Officially Supported Compilers

This project requires a compiler with decent C++17 support.  While other compilers may work, the officially supported compilers can be found below.

* gcc 7 or later
* clang 5 or later
* Xcode 10.3 or later (targeting macOS 10.12 or later)
* MSVC 19.16 or later
