# What is EDK2?

EDK2 is a "development environment" for UEFI firmware. It provides a set of libraries, something of a framework, and a build system for the purpose of developing whole UEFI firmware images.

EDK2 is composed of "packages" which provide various resources to help build a firmware. Somewhat confusingly, the firmware image requirement will be specified in a 'package' and can use pieces of other packages.

For example, the MdePkg provides most of the 'standard libraries' for UEFI development, and the OvmfPkg specifies the components for building a OVMF firmware image, where some of those components are included in the OvmfPkg, and some are from other packages such as the MdePkg. (OVMF is the specification for building a UEFI firmware intended to be run in a VM. This is further explained later in this document)

## Resources

https://www.tianocore.org/
https://github.com/tianocore/tianocore.github.io/wiki/EDK-II
https://github.com/tianocore-training/Tianocore_Training_Contents/wiki

# Initial Setup
## Development Machine Setup

Setting up a development VM is fairly simple:

1. Configure an Ubuntu 18.04 VM
2. Install the following packages installed: `build-essential uuid-dev iasl git gcc-5 nasm python3-distutils python3-dev qemu-system`  
For cross-compiling for ARM also install the packages: `gcc-arm-linux-gnueabi gcc-arm-linux-gnueabihf`
3. Clone the EDK2 repo:  
`git clone https://github.com/tianocore/edk2`
4. Fetch the git submodues:  
`git submodules update --init`

You may want to clone each release to a different folder just to keep things logically separated. If so, the clone command would look something like:

`git clone --depth 1 -b edk2-stable202102 <github_url> edk2-stable202102`

Ideally you'd put all these clones into the same directory, say $HOME/EDK

Once you have the dependencies installed, and the source code cloned, you will need to perform a couple more steps prior to building firmware. (From the Common Instructions for Unix)

**Build the BaseTools**

```bash
cd ~/path/to/edk2
make -C BaseTools
```

**Configure your Environment**

The following commands should be added to your shell config file (for example .bashrc) or will need to be performed each time you start a new terminal.

```bash
export EDK_TOOLS_PATH=$HOME/src/edk2/BaseTools
source $HOME/src/edk2/edksetup.sh BaseTools
```


**Generalizing for multiple versions**

As described in the note above, you may have cloned more than tagged release of edk2 to build different versions for comparison. The best way to handle this is to generalize the above commands into a shell script that can be sourced from each individual build.

Assuming all of your cloned directories are in the same $HOME/EDK path, then in $HOME/EDK add the following to a shell script file named for example setup.sh:

```bash
export EDK_TOOLS_PATH=$(pwd)/BaseTools
source $(pwd)/edksetup.sh BaseTools
```

To start building, change directories into the source root of whatever version you want, the source your setup script:

```bash
cd $HOME/EDK/edk-stable202102
source ../setup.sh
```

You should get messages in your terminal about setting WORKSPACE, TOOLS PATH, and other stuff if it works. If so, congrats! and continue below.

## Firmware Build

Once you have a development VM set up, you can test your environment by building an OVMF firmware image.

OVMF is a project to enable UEFI support for virtual machines. The EDK2 source contains a package 'OvmfPkg' which specifies a firmware build which can be run using QEMU.


To build the OVMF firmware image, run the following build command in the EDK2 source directory:

```bash
build -a X64 -b DEBUG -p OvmfPkg/OvmfPkgX64.dsc -t GCC5
```

where:

`-a X64` instructs the build system to produce a 64bit firmware

`-b DEBUG` builds the DEBUG target, which will contain debug symbols

`-p OvmfPkg/OvmfPkgX64.dsc` specifies the firmware description file to build

`-t GCC5` specifies the toolchain to be used.

Alternatively update the Conf/target.txt file according to the tianocore wiki page [How to build OVMF](https://github.com/tianocore/tianocore.github.io/wiki/How-to-build-OVMF). 

The output of the build command will be in a newly created 'Build' directory in the EDK2 source folder.

The firmware image will be `~/src/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd`

## Running the Firmware with QEMU

Running this firmware file with QEMU is straight forward.

```bash
qemu-system-x86_64 -bios ~/src/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd -net none
```

You should have a QEMU window open, eventually displaying the Tianocore logo and then dropping into a UEFI shell. The default file seems to try to boot from the network and hang, so by turning off net with the command switch, it will actually quickly boot then drop into UEFI shell.

## Resources

https://github.com/tianocore/edk2
https://github.com/tianocore/tianocore.github.io/wiki/OVMF
http://www.linux-kvm.org/page/OVMF

https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II
https://github.com/tianocore/tianocore.github.io/wiki/Using-EDK-II-with-Native-GCC (until Common Instructions)
https://github.com/tianocore/tianocore.github.io/wiki/Common-instructions
https://github.com/tianocore/tianocore.github.io/wiki/How-to-build-OVMF
https://github.com/tianocore/tianocore.github.io/wiki/How-to-run-OVMF

# Module Development

The [Module Writer's Guide](https://edk2-docs.gitbook.io/edk-ii-module-writer-s-guide/) serves as the primary reference for most of the following.

Additionally, various [EDK2 Specifications](https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documentation#specifications) provide additional information about the various files required by the build system.

The example [SimplePkg](../SimplePkg) in this repository is available for following along.

## Packages

Packages are described primarily by two files:  
[.DEC files](https://edk2-docs.gitbook.io/edk-ii-dec-specification/)  
[.DSC files](https://edk2-docs.gitbook.io/edk-ii-dsc-specification/)

A common directory structure for a simple package is as follows:

    CustomPkg
        CustomPkg.dec
        CustomPkg.dsc
        Include
            Protocol
            Guid
            Library
        Application
            CustomApp

## Uefi Applications
### Setup
#### INF File

You first need to create an [INF file](https://edk2-docs.gitbook.io/edk-ii-inf-specification/) for your application. The INF file describes the application to the build system.

The most basic INF file looks like:
```
[Defines]
  INF_VERSION                = 0x0001001B
  BASE_NAME                  = CustomApp
  FILE_GUID                  = XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
  MODULE_TYPE                = UEFI_APPLICATION
  ENTRY_POINT                = CustomAppEntryPoint

[Sources]
  CustomApp.c

[Packages]
  MdePkg/MdePkg.dec

[LibraryClasses]
  UefiLib
  UefiApplicationEntryPoint
```

The INF specification explains the various sections, but a couple things of note regarding the example:

- The paths to the files listed under `[Sources]` are relative to the location of the INF file
- The MdePkg under `[Packages]` provides the libraries listed under `[LibraryClasses]`
- The libraries listed under `[LibraryClasses]` are likely to be the minimum set of libraries for a UEFI application  
  (Unless the application doesn't do anything at all, then you will just need the `UefiApplicationEntryPoint` library)

#### Source Files and Entry Point

You will need to at least implement the `ENTRY_POINT` in the source file listed.

For example, for the INF file above, `CustomApp.c` might look like the following for a 'hello world' application:

```C
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>

EFI_STATUS
EFIAPI
CustomAppEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    CHAR16 hello_str[] = L"Hello World, from my uefi app\n";
    Print(hello_str);

    return EFI_SUCCESS;
}
```

The result in the example directory structure from the Packages section above might look like:

    CustomPkg
        CustomPkg.dec
        CustomPkg.dsc
        Include
            Protocol
            Guid
            Library
        Application
            CustomApp
                CustomApp.inf
                CustomApp.c
### Build

To build the application, you will need to list the INF file for your application under the target package's DSC file `[Components]` section.

In the case of building the 64bit OVMF firmware, this means adding the INF path to the end of the `OvmfPkg/OvmfPkgX64.dsc` file.

For the working example, this path would be:  
`CustomPkg/Application/CustomApp/CustomApp.inf`  
Note that this path is relative to the EDK2 source directory

Once you have added the INF path, you can rebuild the firmware image by running the build command again.

The output from the build process will be in:  
`~/src/edk2/Build/OvmfX64/DEBUG_GCC5/X64/`

For the example, the files would be:
`~/src/edk2/Build/OvmfX64/DEBUG_GCC5/X64/CustomApp.efi`
`~/src/edk2/Build/OvmfX64/DEBUG_GCC5/X64/CustomApp.debug`


### Running an UEFI Application

To use QEMU to run your application, the easiest way is to use QEMU's ability to use host directories as disk images.

Create a directory, such as `~/hda-contents`, to store the application files.


Copy the application files to the newly created directory:

```bash
cp ~/src/edk2/Build/OvmfX64/DEBUG_GCC5/X64/CustomApp.* hda-contents
```

Run QEMU with the OVMF firmware image as the bios, with your new directory as a read/write hard disk:

```bash
qemu-system-x86_64 -bios /path/to/OVMF.fd -hda fat:rw:~/hda-contents
```

You should eventually be dropped into a UEFI shell. This shell operates somewhat like the Windows CMD Prompt.

Change into the FS0 drive
```
Shell> FS0:
FS0>
```
Execute the application
```
FS0> CustomApp
Hello World, from my uefi app
FS0>
```

## Uefi Drivers
TO DO

## DXE Modules
TO DO

## PEIM Modules
TO DO

## SEC Modules
TO DO

## Resources

https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documents  
https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Libraries-and-Helper-files  
https://github.com/tianocore/tianocore.github.io/wiki/UDK2018#documentation  

https://tianocore-docs.github.io/edk2-UefiDriverWritersGuide/draft/  
https://edk2-docs.gitbook.io/edk-ii-module-writer-s-guide/  

https://edk2-docs.gitbook.io/edk-ii-inf-specification/2_inf_overview  


# Testing and Debugging
## Setup

**Step 1**

In order to use GDB, you will need to compile from source to enable a couple of options

Download a [GDB source release](http://ftp.gnu.org/gnu/gdb/) (tested with GDB-8.2 and GDB 10.1) and extract, and from the GDB source directory execute the following:
```bash
mkdir build && cd build
../configure --with-python=python3 --with-expat --target=x86_64-w64-mingw32
make
sudo make install
```
Note that compiling GDB to use Python3 seems to be necessary


**Step 2**

Set up a couple of named pipes for the UDK Debugger Tool:
```bash
mkfifo /path/to/named_pipe.in
mkfifo /path/to/named_pipe.out
```
Note the '.in' and '.out'


**Step 3**

Next you will need to install the UDK Debugger Tool from intel: https://software.intel.com/sites/default/files/managed/d0/98/UDK_Debugger_Tool_v1_5_1_Linux.zip, and execute the following:

```bash
unzip UDK_Debugger_Tool_v1_5_1_Linux.zip
chmod +x UDK_Debugger_Tool_v1_5_1.bin
sudo ./UDK_Debugger_Tool_v1_5_1.bin
```
(Note that sudo privileges are only required to install to the default location)


An InstallAnywhere wizard should open, and for the most part you can click through with the defaults, except when configuring the debug port.

When you get to the Configure Debug Port page, select the Pipe option from the drop down (the default is to use a serial device).

Specify the path to the named pipe you created in Step 2, but without the 'in' or 'out': /path/to/named_pipe


If you need to change any options, just rerun the installer.


**Step 4**

Lastly you will need to build the OVMF firmware with SOURCE_DEBUG_ENABLE set.

You can do this by either modifying the OvmfPkgX64.dsc file and setting the SOURCE_DEBUG_ENABLE variable to TRUE, and then rebuilding.  
Or just rebuild running the build command with -D SOURCE_DEBUG_ENABLE:

```bash
build -a X64 -b DEBUG -p OvmfPkg/OvmfPkgX64.dsc -t GCC5 -D SOURCE_DEBUG_ENABLE
```

This will enable the various debug modules described in the [UDK Debugger Manual](https://software.intel.com/content/dam/develop/external/us/en/documents/udk-debugger-tool-user-manual-v1-11-820238.pdf)

## Running the Debugger

To debug the OVMF firmware with GDB, perform the following

1. Execute the udk-gdb-server (at the following path if installed to the default location). The GDB server will start, and wait for QEMU to connect to the named pipe. 
```bash  
/opt/intel/udkdebugger/bin/udk-gdb-server  
```  

2. Start QEMU with a serial port forwarded to the named pipe:

```bash
qemu-system-x86_64 -bios /path/to/bios/firmware_file -hda fat:rw:/path/to/contents/directory -serial pipe:/path/to/pipe_name
```

3. Connect using your compiled GDB:

```
gdb> target remote :1234
gdb> source /opt/intel/udkdebugger/script/udk_gdb_script
(udb)>
```

4. You should now be able to set breakpoints for symbols from the OVMF firmware.


Note that to debug a UEFI application, you will need to ensure that GDB properly loads the image symbols. The UDK Debugger Tool (gdb server + script) helps with this.

## Resources

https://software.intel.com/content/www/us/en/develop/articles/unified-extensible-firmware-interface.html
https://software.intel.com/content/dam/develop/external/us/en/documents/udk-debugger-tool-user-manual-v1-11-820238.pdf
https://jp3bgy.github.io/blog/uefi/2018/12/24/How-to-Source-Debug-OVMF.html (https://translate.google.com/translate?sl=auto&tl=en&u=https://jp3bgy.github.io/blog/uefi/2018/12/24/How-to-Source-Debug-OVMF.html)


# QEMU+UEFI Cheat Sheet

Full Command  
```bash
qemu-system-x86_64 -bios /path/to/bios/firmware_file \
				   -hda fat:rw:/path/to/contents/directory \
				   -net none \
                   -debugcon file:debug.log -global isa-debugcon.iobase=0x402
                   -serial pipe:/tmp/pipe_name
```



To execute a 'bios' firmware:  
`qemu-system-x86_64 -bios /path/to/bios/firmware_file`

To enable a drive from a directory:  
`-hda fat:rw:hda-contents`

To disable network interfaces:  
`-net none`

To enable debug output:  
`-debugcon file:debug.log -global isa-debugcon.iobase=0x402`

To enable serial output (to a named pipe):  
`-serial pipe:/tmp/pipe_name`



