FROM resin/rpi-raspbian:stretch

RUN apt-get update \ 
    && apt-get upgrade -y \ 
    && apt-get install libspdlog-dev git wget gcc g++ cmake make -y

RUN mkdir -p /usr/src/bcm2835 \
    && wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz \
    && tar -C /usr/src/bcm2835 -xvf bcm2835-1.56.tar.gz \
    && rm bcm2835-1.56.tar.gz \
    && cd /usr/src/bcm2835/bcm2835-1.56/ \
    && ./configure \
    && make \
    && sudo make install

RUN git clone https://github.com/intel-iot-devkit/mraa.git \
    && cd mraa \
    && mkdir build \
    && cd build \
    && cmake .. -DBUILDSWIGNODE=OFF \
    && sudo make install

RUN echo "/usr/local/lib/arm-linux-gnueabihf " >> /etc/ld.so.conf \
    && sudo ldconfig 

RUN git clone https://github.com/tmrh20/RF24.git RF24 \
    && cd RF24 \
    && sudo make install

RUN git clone https://github.com/tmrh20/RF24Network.git RF24Network \
    && cd RF24Network \
    && sudo make install

COPY main.cpp /usr/src/receiver/main.cpp

RUN cd /usr/src/receiver \
    && g++ main.cpp -o main

