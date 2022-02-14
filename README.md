# leaf - The AcaciaLinux package manager

This is the package manager made for AcaciaLinux, written in C++ and supporting a really simple packaging format

# WARNING

leaf is a work in progress project that is **<u>*FAR*</u>** from finished. leaf is now **<u>ARMED</u>** (yes we mean that), it has any safety paths pointed to their final destination, means:

- Packages are installed to your ROOT (yes)

<<<<<<< HEAD
- /etc/laef will be manipulated
=======
- /etc/leaf will be manipulated
>>>>>>> ea135fe (ARMED leaf)

- /var/cache/leaf will be manipulates

- Anything else install scripts may do

# Installation

### Dependencies

leaf currently has to be built from scratch you will need the following dependencies installed on your build and run environment:

```bash
libcurl
libarchive
```

#### Cloning

Once you have installed these dependencies, you can clone the project and enter the directory:

```bash
git clone --recursive https://github.com/AcaciaLinux/leaf
cd leaf
```

### Building

Using the following commands you can create the build directory:

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

You may replace <jobs> with your amount of jobs to use

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

# # Filesystem

leaf configuration files live in /etc/leaf/, currently there are following files:

- packages.list (downloadable by leaf)

- leaf.conf (currently not needed)

In order for leaf to work correctly, the upper files have to exist or have to be downloaded.

#### packages.list

The packages.list file can be downloaded automatically by leaf using the following command: "leaf update": There is no need to create this file manually. The command will dowload the file from the remote server so leaf always has up to date lists.