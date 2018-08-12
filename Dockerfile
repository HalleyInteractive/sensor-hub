FROM resin/rpi-raspbian:wheezy
RUN apt-get update && \
apt-get upgrade -y && \ 
apt-get install libspdlog-dev git wget gcc -y

RUN mkdir -p /usr/src/bcm2835 \
    && wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz \
    && tar -C /usr/src/bcm2835 -xvf bcm2835-1.56.tar.gz \
    && rm bcm2835-1.56.tar.gz \
    && ./usr/src/bcm2835/bcm2835-1.56/configure \
    && sudo make -C /usr/src/bcm2835/bcm2835-1.56 all \
    && sudo make install /usr/src/bcm2835/bcm2835-1.56

RUN git clone https://github.com/intel-iot-devkit/mraa.git \
&& cd mraa \
&& mkdir build \
&& cd build \
&& cmake .. -DBUILDSWIGNODE=OFF \
&& sudo make install

# RUN nano /etc/ld.so.conf \
# && /usr/local/lib/arm-linux-gnueabihf 
# sudo ldconfig 

# # nrf modules
# ./configure --driver=MRAA
# sudo make install -B