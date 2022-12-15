#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include "rpi_ws281x/clk.h"
#include "rpi_ws281x/gpio.h"
#include "rpi_ws281x/dma.h"
#include "rpi_ws281x/pwm.h"
#include "rpi_ws281x/ws2811.h"

static volatile int quit;



/* leds */



#define TARGET_FREQ WS2811_TARGET_FREQ
#define GPIO_PIN 12
#define DMA 10

#define LED_COUNT 1
#define RGB(R, G, B) (((R)) | ((G)) | (B))

static ws2811_t ledstring =
    {
        .freq = TARGET_FREQ,
        .dmanum = DMA,
        .channel =
            {
                [0] =
                    {
                        .gpionum = GPIO_PIN,
                        .count = LED_COUNT,
                        .invert = 0,
                        .brightness = 100,
                    },
            },
};
static volatile ws2811_led_t leds[LED_COUNT];

// static void handleleds();

static void handleleds()
{
    int i;
    ws2811_return_t ret;
    const ws2811_led_t colors[8] = {
        0x00200000, // red
        0x00201000, // orange
        0x00202000, // yellow
        0x00002000, // green
        0x00002020, // lightblue
        0x00000020, // blue
        0x00100010, // purple
        0x00200010, // pink
    };
    ws2811_init(&ledstring);

    while (1)
    {
        // ledstring.channel[0].leds[i] = colors[i];
        // int r = (colors[3] >> 16) & 32;
        // int g = (colors[3] >> 8) & 32;
        // int b = (colors[3]) & 32;
        // ledstring.channel[0].leds[0] = (((0x0000ff & 0x0000ff) >> 4) << 16) | (((0x0000ff & 0x0000ff) >> 4) << 8) | (((0x0000ff & 0xff0000 >> 16) << 4));
         ledstring.channel[0].leds[0] = 0xFFFF0000;

        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
            break;
        }
        usleep(5);
    }
    ws2811_fini(&ledstring);
    printf("\nws2811_fini\n");
}

int main(int argc, char *argv[])
{
    handleleds();
    return 0;
}