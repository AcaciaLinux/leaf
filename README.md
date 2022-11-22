# leaf - The AcaciaLinux package manager

This is the package manager made for AcaciaLinux, written in C++ and supporting a really simple packaging format

# For guides, you may refer to the [WIKI](https://github.com/AcaciaLinux/leaf/wiki)

# Quick Links:

- [Configuration](https://github.com/AcaciaLinux/leaf/wiki/Configuration)

# Installation

#### Dependencies

leaf currently has to be built from scratch. You will need the following dependencies installed on your build and run environment:

```bash
libcurl
libarchive
ncurses
```

#### Cloning

Once you have installed these dependencies, you can clone the project and enter the leaf directory:

```bash
git clone --recursive https://github.com/AcaciaLinux/leaf
cd leaf
```

#### Building

Using the following commands you can create a build directory:

```bash
mkdir build
cd build
```

Use the following commands to build leaf:

```bash
cmake ..
make -j <jobs>
```

If you want to build tests too, use

```bash
cmake -DBUILD_TESTS=ON ..
make -j <jobs>
```

You may replace `<jobs>` with your amount of jobs to use

#### Installing

Finally you can install leaf on your system:

```bash
make install
```

#### Cleanup

If you want to clear the space, you can do so by removing the leaf directory:

```bash
rm -rf leaf
```
