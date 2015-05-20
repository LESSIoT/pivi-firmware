# Build pivi-firmware inside docker

The provided Dockerfile creates an Ubuntu 14.04.2 installation with a
manually built avr-libc 1.8.1.

This instructions have been tested on Debian 8

## Create the docker image

Install docker:

    sudo apt-get install docker.io

Clone this repo and:

    cd docker
    sudo docker build -t avr .

Wait for docker to finish.... and done!

## Building the firmware

Once the docker image is created, from the top directory of this
repository:

    source docker/setup.sh
    docker_make

The source is only needed once per terminal used. After `docker_make`
any arguments may be added.

