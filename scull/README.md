# scull

**S**imple **C**haracter **U**tility for **L**oad **L**ocalities - character device driver that can store text data.
**Note:** linux driver - is a kernel module, that dynamically linked in to kernel, any wrong actions can **damage** Your system, so it **strongly recommended** to use virtualization software, like Virtual Box or anothers with pre-installed linux distribution. 

## Kernel version
4.13.0-39-generic

## Pre-release notes:
*  v0.1 - initial driver version. Can read and write text data, realized by storing data in static character array

## How to download and build:
1. Please be sure that on Your system installed linux-kernel headers, with version, that correspond to version of linux kernel that You use.
>  For example for me it was: `/usr/src/linux-headers-4.10.0-42-generic` on Ubuntu You can download it via `sudo apt-get install linux-headers-<kernel_version>-generic`,
>  You can check Your kernel version via `uname -r`.
2. Download the repository : `git clone https://github.com/AndriyVeres/LDD.git`
3. Checkout to release branch: `git checkout master`
4. Change **KDIR** variable in Makefile, with respect of location linux kernel heades and version in Your system, for example for me it was: `KDIR:= /usr/src/linux-headers-4.10.0-42-generic`
5. Type `make`, and check the console output, in successfull case it must be something like:

```
root@XLDD:/usr/src# make
make -C /usr/src/linux-headers-4.10.0-42-generic M=`pwd`
make[1]: Entering directory '/usr/src/linux-headers-4.10.0-42-generic'
 Building modules, stage 2.
 MODPOST 1 modules
make[1]: Leaving directory '/usr/src/linux-headers-4.10.0-42-generic'
root@XLDD:/usr/src#
```
## How to use
All things, excluding read/write requres superuser privilegies.
1. Insert scull.ko in to kernel: `insmod scull.ko`
2. Check the kernel log, for example for me it was: `tail -f /var/log/kern.log`

```
[ 7586.322323] SCULL: device number registration...
[ 7586.322332] SCULL: device: F500000 MAJOR: 245 MINOR: 0 count: 1
[ 7586.322334] SCULL: success device number registration
```
3. Create device node, that will be correspond to driver by typing: `mknod /dev/scull c <MAJOR> <MINOR>`, where **MAJOR** and **MINOR** are the numbers , that You can see in kernel log, for example for me it was: `mknod /dev/scull c 245 0`
4. Change the access rights for /dev/scull as You need via **chmod**, for example: `chmod 777 /dev/scull`
5. You can read/write data by using standard linux utilities, like **cat** and **echo** or write simple userspace program, that use device via **fread(), fwrite(), fopen(), fclose()** functions, for example:

```
root@XLDD:/usr/src# echo "Hello scull device driver" > /dev/scull
root@XLDD:/usr/src# cat /dev/scull
Hello scull device driver
root@XLDD:/usr/src#
```
6. After the end of using remove from kernel scull.ko: `rmmod scull` and check the kernel log, it must be something like:

```
[ 7603.451232] SCULL: device number deregistration...
[ 7603.451236] SCULL: success device number deregistration
```