FROM resin/rpi-raspbian
RUN apt-get update && \
apt-get upgrade -y && \ 
apt-get install libspdlog-dev git -y

RUN mkdir -p /usr/src/bcm2835 \
    && curl -SL http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz \
    | tar -xvf /usr/src/bcm2835 \
    && ./usr/src/bcm2835/configure \
    && sudo make -C /usr/src/bcm2835 all \
    && sudo make install /usr/src/bcm2835

# git clone https://github.com/intel-iot-devkit/mraa.git
# cd mraa
# mkdir build
# cd build
# cmake .. -DBUILDSWIGNODE=OFF
# sudo make install

# nano /etc/ld.so.conf 
# /usr/local/lib/arm-linux-gnueabihf 
# sudo ldconfig 


# # nrf modules
# ./configure --driver=MRAA
# sudo make install -B