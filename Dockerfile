FROM resin/rpi-raspbian:stretch

RUN apt-get update \ 
    && apt-get upgrade -y \ 
    && apt-get install git wget gcc g++ cmake make -y

RUN mkdir -p /usr/src/bcm2835 \
    && wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz \
    && tar -C /usr/src/bcm2835 -xvf bcm2835-1.56.tar.gz \
    && rm bcm2835-1.56.tar.gz \
    && cd /usr/src/bcm2835/bcm2835-1.56/ \
    && ./configure \
    && make \
    && sudo make install
    # rm bcm

RUN git clone --depth 1 https://github.com/intel-iot-devkit/mraa.git \
    && cd mraa \
    && mkdir build \
    && cd build \
    && cmake .. -DBUILDSWIGNODE=OFF \
    && sudo make install
    # rm mraa

RUN echo "/usr/local/lib/arm-linux-gnueabihf " >> /etc/ld.so.conf \
    && sudo ldconfig 

RUN git clone --depth 1 https://github.com/nRF24/RF24.git RF24 \
    && cd RF24 \
    && sudo make install -B
    # rm RF24

RUN git clone --depth 1 https://github.com/nRF24/RF24Network.git RF24Network \
    && cd RF24Network \
    && sudo make install -B
    # rm RF24Network

RUN git clone --depth 1 https://github.com/gabime/spdlog.git SPDLOG \
    && mv SPDLOG/include/spdlog /usr/include/
    # && cmake -H. -B_builds -DCMAKE_BUILD_TYPE=Release \
    # && cmake --build _builds --target install
    # rm SPDLOG

WORKDIR /usr/src/receiver
COPY main.cpp /usr/src/receiver
RUN cd /usr/src/receiver \
    && g++ -Wall -std=c++11 -lrf24-bcm -lrf24network -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv7-a -mtune=arm1176jzf-s -lrf24network -lpthread -g main.cpp -o main
