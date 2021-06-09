# LibreOffice Convertor (loconvertor)

This is a simple C++ application that can convert ODT or other document files
using LibreOffice SDK into PDF or other LibreOffice supported file types. This
application is using `qmake` or `cmake` in order to compile, execute and debug
easier using Qt Creator. To keep it simple, the arguments are not processed,
and the input file is the `~/test.odt`, and the output will be `~/test.pdf`.

## Compiling and Running

### Using Official SDK

To be able to compile and run this example, you should have installed
LibreOffice and LibreOffice SDK, then you should set `LOROOT` in
`loconvertor.pro` to appropriate folder. For LibreOffice 7.1 SDK, you should
use this line:

    LOROOT = /opt/libreoffice7.1

Compiling and running the `loconvertor` is easy. First, run an instance of
LibreOffice to listen for the incoming connections:

    $ libreoffice7.1 "--accept=socket,port=2083;urp;"
    
and then just open the project file in Qt Creator, and click `Run` or press
`Ctrl+R`. Both `cmake` and `qmake` should can be used.

### Local Build

If you have [built LibreOffice](
https://wiki.documentfoundation.org/Development/BuildingOnLinux) yourself, use
the `instdir` path for `LOROOT`:

    LOROOT = /home/hossein/Projects/libreoffice/core/instdir

In such case, the above symbolic link is not needed, and should not be created.

If you use a local build, run:

    $ ./instdir/program/soffice.bin "--accept=socket,port=2083;urp;"
    
And execute the project from Qt Creator.

## Building and Running from Command Line using cmake

To run `loconvertor` from command line using cmake, go to the source folder,
and then:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./loconvertor
