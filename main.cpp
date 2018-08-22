#include <iostream>
#include "RF24/RF24.h"
#include "RF24Network/RF24Network.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
// #include "bcm2835.h"

int main () {
  RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
  RF24Network network(radio);

  const uint16_t this_node = 00;

  struct payload_t {
    uint16_t id;
    uint16_t temperature;
    uint16_t humidity;
  };

  auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily_log", 0, 0);
  spdlog::set_pattern("[%H:%M:%S %z] [%n] %v");

  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink->set_level(spdlog::level::warn);
  console_sink->set_pattern("[STDOUT][%H:%M:%S %z] [%n] %v");

  auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/debug.log", true);
  file_sink->set_level(spdlog::level::trace);
  file_sink->set_pattern("[LOG][%H:%M:%S %z] [%n] %v");

  spdlog::logger logger("MULTI", {console_sink, file_sink});
  logger.set_level(spdlog::level::debug);
  logger.flush_on(spdlog::level::err);

  logger.info("Starting radio");
  try {
    radio.begin();
  } catch (int e) {
    logger.warn("Could not start the radio, sudo permissions?");
    std::cout << "Could not start the radio. " << e << std::endl;
  }
  logger.trace("Radio started");

  delay(5);
  logger.trace("Delayed 5 seconds");
  network.begin(90, this_node);
  logger.trace("Network starting");
  radio.printDetails();
  logger.trace("Radio details printed");

  while(1) {
    network.update();
    while ( network.available() ) {
      RF24NetworkHeader header;

      payload_t payload;
      network.read(header, &payload, sizeof(payload));

      float f_temperature = (float)payload.temperature / 10;
      float f_humidity = (float)payload.humidity / 10;
      std::cout << "NODE:" << payload.id << " TEMP:" << f_temperature << " HUM:" << f_humidity
      daily_logger->info("NODE:{0:d} TEMP:{1:f} HUM:{2:f}", payload.id, f_temperature, f_humidity);
    }
    delay(2000);
  }
}