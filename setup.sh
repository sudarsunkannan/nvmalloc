export NVMALLOC_HOME=`$PWD`

#Install packages
sudo apt-get install libssl-dev

#setup tmpfs with size in megabytes
scripts/setuptmpfs.sh 4096
