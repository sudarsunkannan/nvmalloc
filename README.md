nvmalloc library - Under development, cleanup, and restructuring phase 
---------------------------------------------------------------------

## How to build

To compile the library, you need the following libraries


     $ cd nvmalloc
     $ export NVMALLOC_HOME=$PWD
     $ sudo apt-get install libssl-dev

To build the object library,

    $ make clean
    $ make 

# Provides pesistent allocation

# uses Intel's NVML for logging
