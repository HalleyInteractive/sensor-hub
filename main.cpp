#include "RF24/RF24.h"
#include "RF24Network/RF24Network.h"
#include "spdlog/spdlog.h"
#include "bcm2835.h"

int main () {
  RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
  RF24Network network(radio);

  const uint16_t this_node = 00;

  struct payload_t {
    uint16_t id;
    uint16_t temperature;
    uint16_t humidity;
  };

  radio.begin();
  delay(5);
  network.begin(90, this_node);
  daily_logger->info(radio.printDetails());

  auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily.log", 0, 0);
  spdlog::set_pattern("[%H:%M:%S %z] [%n] %v");

  daily_logger->info("APP STARTING");

  while(1) {
    network.update();
    while ( network.available() ) {
      RF24NetworkHeader header;
      
      payload_t payload;
      network.read(header, &payload, sizeof(payload));
    
      float f_temperature = (float)payload.temperature / 10;
      float f_humidity = (float)payload.humidity / 10;
      
      daily_logger->info("NODE:{0:d} TEMP:{1:f} HUM:{2:f}", payload.id, f_temperature, f_humidity);
    }
    delay(2000);
  }
}