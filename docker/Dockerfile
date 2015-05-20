FROM ubuntu:14.04.2

RUN \
  apt-get update && \
  apt-get -y upgrade && \
  apt-get install -y build-essential && \
  apt-get install -y gcc-avr && \
  apt-get install -y wget && \
  apt-get install -y unzip bc

RUN wget http://download.savannah.gnu.org/releases/avr-libc/avr-libc-1.8.1.tar.bz2
RUN tar xjf avr-libc-1.8.1.tar.bz2
RUN \
  cd avr-libc-1.8.1 && \
  ./configure --prefix=/usr/lib  --build=`./config.guess` --host=avr && \
  make -j4 && \
  make install

RUN useradd -ms /bin/bash user
USER user
RUN cd && \
  wget http://www.atmel.com/images/asf-standalone-archive-3.21.0.6.zip && \
  unzip asf-standalone-archive-3.21.0.6.zip
