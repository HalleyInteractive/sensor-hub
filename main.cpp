#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include "RF24/RF24.h"
#include "RF24Network/RF24Network.h"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"

RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
RF24Network network(radio);
const uint16_t this_node = 00;
struct payload_t {
  uint16_t id;
  uint16_t temperature;
  uint16_t humidity;
};

int main () {

  spdlog::set_pattern("[%H:%M:%S %z] [%n] %v");
  // spdlog::set_async_mode(4096);
  spdlog::flush_on(spdlog::level::info);

  auto debug_logger = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>("debug", "logs/debug.log");
  auto sensor_logger = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>("sensor", "logs/sensor.log");

  debug_logger->info("Starting radio");
  try {
    debug_logger->info("Starting begin");
    radio.begin();
  } catch (int e) {
    debug_logger->warn("Could not start the radio, sudo permissions?");
    std::cout << "Could not start the radio. " << e << std::endl;
  }

  debug_logger->info("Radio started");

  std::this_thread::sleep_for(std::chrono::seconds(5));
  debug_logger->info("Delayed 5 seconds");
  network.begin(90, this_node);
  debug_logger->info("Network starting");
  
  // std::stringstream ss;
  // auto old_buf = std::cout.rdbuf(ss.rdbuf()); 
  radio.printDetails();
  // std::cout.rdbuf(old_buf);
  // debug_logger->info(ss.str());
  debug_logger->info("Radio details printed");

  std::cout << "Network started, listening for data" << std::endl;

  while(1) {
    network.update();
    while ( network.available() ) {
      debug_logger->info("Network available");
      RF24NetworkHeader header;

      payload_t payload;
      network.read(header, &payload, sizeof(payload));

      float f_temperature = (float)payload.temperature / 10;
      float f_humidity = (float)payload.humidity / 10;
      // std::cout << "NODE:" << payload.id << " TEMP:" << f_temperature << " HUM:" << f_humidity;
      debug_logger->info("NODE:{0:d} TEMP:{1:.2f} HUM:{2:.2f}", payload.id, f_temperature, f_humidity);
      sensor_logger->info("NODE:{0:d} TEMP:{1:.2f} HUM:{2:.2f}", payload.id, f_temperature, f_humidity);
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
}