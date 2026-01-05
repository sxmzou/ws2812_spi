/*
*
*   file: main.c
*   update: 2025-01-05
*   usage:
*       make
*       sudo ./main
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#include "spi.h"
#include "test.h"

#define SPI_BUS         "/dev/spidev0.0"
#define LED_NUM         20*48

unsigned char send_buf[24 * LED_NUM];

/*****************************
 * @brief : 将RGB888数据转换为SPI发送缓冲区
 * @param : rgb888_data - RGB888格式的LED数据 (576 * 3 字节)
 *          spi_buf     - SPI发送缓冲区 (576 * 24 字节)
 *****************************/
void rgb888_to_spi_buffer(const uint8_t *rgb888_data, unsigned char *spi_buf)
{
    for (int led = 0; led < LED_NUM; led++) {
        int offset = led * 24;
        int data_offset = led * 3;

        unsigned char r = rgb888_data[data_offset];     // Red
        unsigned char g = rgb888_data[data_offset + 1]; // Green
        unsigned char b = rgb888_data[data_offset + 2]; // Blue

        // Green (bits 0-7) - WS2812B格式: GRB
        for (int i = 0; i < 8; i++) {
            spi_buf[offset + i] = (g & 0x80) ? 0xFC : 0xC0;
            g <<= 1;
        }

        // Red (bits 8-15)
        for (int i = 8; i < 16; i++) {
            spi_buf[offset + i] = (r & 0x80) ? 0xFC : 0xC0;
            r <<= 1;
        }

        // Blue (bits 16-23)
        for (int i = 16; i < 24; i++) {
            spi_buf[offset + i] = (b & 0x80) ? 0xFC : 0xC0;
            b <<= 1;
        }
    }
}

int main(int argc, char **argv)
{
    int ret;
    spi_operations_t *spi_ops;

    // 初始化SPI
    ret = spi_init(SPI_BUS);
    if(ret < 0) {
        printf("SPI init failed!\n");
        return -1;
    }
    spi_ops = get_spi_ops();

    printf("Starting LED expression display...\n");
    printf("LED count: %d\n", LED_NUM);
    printf("Data source: test.h expression_data_rgb888\n");

    // 将RGB888表情数据转换为SPI发送缓冲区
    rgb888_to_spi_buffer(expression_data_rgb888, send_buf);

    // 持续刷新显示 (30fps)
    while(1)
    {
        spi_ops->spi_write_nbyte_data(send_buf, sizeof(send_buf));
        usleep(33333);  // 30fps = 1000ms/30 ≈ 33.3ms
    }

    return 0;
}
