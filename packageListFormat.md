# Package list format

Leaf uses a very simple package and package list format, it is human readable and uses a very simple syntax to make it easy and fast to parse

# Base format

(package name);(package version string);package description;[dependency1][dependency2];(downstream url)

### Example:

seed;0.1;The AcaciaLinux installer;[python][gtk];https://.../AcaciaLinux/seed/.../

leaf;0.1;The AcaciaLinux package manager;[gcc][cmake];https://.../AcaciaLinux/leaf/.../.../

As the format already implies, the use of ';', '[' and ']' is not allowed. These symbols are reserved for the format, we are working on a way to make these symbols usable in later releases
