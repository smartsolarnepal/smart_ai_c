// #define DEVICE_ID 0x53
// #define REG_POWER_CTL   0x2D
// #define REG_DATA_X_LOW  0x32
// #define REG_DATA_X_HIGH 0x33
// #define REG_DATA_Y_LOW  0x34
// #define REG_DATA_Y_HIGH 0x35
// #define REG_DATA_Z_LOW  0x36
// #define REG_DATA_Z_HIGH 0x37

#include <iostream>
#include <thread>
#include <chrono>
#include <wiringPiI2C.h>

#define ADC_CHAN_NUM 8
#define ADC_DEFAULT_IIC_ADDR 0x04
#define REG_RAW_DATA_START 0x10
#define REG_VOL_START 0x20
#define REG_RTO_START 0x30
#define REG_SET_ADDR 0xC0

// function that converts received 16 bits to 12 bits data
int16_t shifter(int16_t data)
{
    data >>= 4;     // 0b11111111111110000 ====> 0bxxxx111111111111
    data &= 0x0fff; // 0bxxxx111111111111 ====> 0b0000111111111
    return data;

    // std::cout << "Final  " << data << "\n" ;
}

int main(int argc, char **argv)
{
    int16_t adc_data[8];
    int16_t offset = 0;

    // Setup I2C communication
    int fd = wiringPiI2CSetup(ADC_DEFAULT_IIC_ADDR);
    if (fd == -1)
    {
        std::cout << "Failed to init I2C communication.\n";
        return -1;
    }
    std::cout << "I2C communication successfully setup.\n";

    while (1)
    {
        // Import the data from ADC register.
        // Adjust the data since the ADC is only 12-bits ADC.
        for (offset = 0; offset <= 7; offset++)
        {
            adc_data[offset] = wiringPiI2CReadReg16(fd, REG_VOL_START + offset);
            // adc_data[offset] = shifter(adc_data[offset]);
        }

        for (offset = 0; offset <= 7; offset++)
        {
            std::cout << adc_data[offset] << " ";
        }

        std::cout << "\n";
    }

    return 0;
}