nvmalloc library - Under development, cleanup, and restructuring phase 
---------------------------------------------------------------------

# How to build

To compile the library, you need the following libraries

     $ cd nvmalloc
     $ export NVMALLOC_HOME=$PWD
     $ sudo apt-get install libssl-dev

To build the object library, use sudo if required in your machine

    $ make clean
    $ make 
    $ make install	

Setup tmpfs with size in megabytes

    $ scripts/setuptmpfs.sh 4096


# How to run (helper script runtests.sh can be also used)

Clean the tmpfs/object folder 
	
    $ scripts/cleantmpfs.sh


Write persistent objects

    $ test/persist_alloc


Write first, and read the persistent objects with length

    $ scripts/cleantmpfs.sh
    $ test/nvm_rw_test w
    $ test/nvm_rw_test r


Snappy compression example. First generate objects and then read and compress.


    $ scripts/cleantmpfs.sh
    $ test/snappy_test w
    $ test/snappy_test r
