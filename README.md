# leaf - The AcaciaLinux package manager

This is the package manager made for AcaciaLinux, written in C++ and supporting a really simple packaging format

# Filesystem

leaf configuration files live in /etc/leaf/, currently there are the following files:

- packages.list (downloadable by leaf)

- leaf.conf (currently not needed)

In order for leaf to work correctly, the upper files have to exist or have to be downloaded.

#### packages.list

The packages.list file can be downloaded automatically by leaf by issuing the following command: "leaf update", so there is no need to create this file manually. The command will dowload the file from the remote server so leaf always knows, which packages are available