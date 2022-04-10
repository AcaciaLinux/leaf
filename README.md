# leaf - The AcaciaLinux package manager

This is the package manager made for AcaciaLinux, written in C++ and supporting a really simple packaging format

# For guides, you may refer to the [WIKI](https://github.com/AcaciaLinux/leaf/wiki)

# WARNING

leaf is a work in progress project that is **<u>*FAR*</u>** from finished. leaf is now **<u>ARMED</u>** (yes we mean that), it has any safety paths pointed to their final destination, means:

- Packages are installed to your ROOT (yes)

- /etc/leaf will be manipulated

- /var/cache/leaf will be manipulated

- Anything else install scripts may do

# Installation

### Dependencies

leaf currently has to be built from scratch. You will need the following dependencies installed on your build and run environment:

```bash
libcurl
libarchive
```

#### Cloning

Once you have installed these dependencies, you can clone the project and enter the leaf directory:

```bash
git clone --recursive https://github.com/AcaciaLinux/leaf
cd leaf
```

### Building

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
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j <jobs>
```

You may replace `<jobs>` with your amount of jobs to use

### Installing

Finally you can install leaf on your system:

```bash
make install
```

### Cleanup

If you want to clear the space, you can do so by removing the leaf directory:

```bash
rm -rf leaf
```

# Files

## Configuration

leaf configuration files live in /etc/leaf/, currently there are following files:

- leaf.pkglist (downloadable by leaf)

- leaf.conf (currently not needed)

In order for leaf to work correctly, leaf will prompt you to create them if they do not exist.

### leaf.pkglist

The leaf.pkglist file can be downloaded automatically by leaf using the following command: `leaf update`. There is no need to create this file manually. The command will download the file from the remote server so leaf always has up to date lists.

## Cache

leaf puts all of its caches into the directory `/var/cache/leaf` in the specified root (normally '/'). Leaf will ask to create these directories if they do not exist. If you need to clear the leaf cache, you can delete everything contained. Currently there are the following directories:

- downloads

- packages

### downloads

This directory stores all the downloaded package archives, these are not extracted and will be re-downloaded if they do not exist.

### packages

This directory contains all the extracted packages. The package directories can be removed to save space, because this is uncompressed data. Once you install a package, it will be extracted into this directory for processing.
