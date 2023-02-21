#include <stdint.h>
#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>
#include <IMU.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;
constexpr uint8_t I2C_SLAVE_ADDR = 0x28;

int main() {
    // Initialize system
    IO::init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    uint8_t chip_id = 0;
    i2c.readReg(I2C_SLAVE_ADDR, 0x00, &chip_id);
    uart.printf("{%x}",chip_id);
    time::wait(500);
}

