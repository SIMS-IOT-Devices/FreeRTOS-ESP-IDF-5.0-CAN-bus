// CAN bus receive using TWAI (Two-Wire Automotive Interface) driver on ESP32 in ESP IDF 5.3

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "driver/twai.h"

void app_main(void)
{
    // Configure TWAI general settings TWAI_GENERAL_CONFIG_DEFAULT(tx_io, rx_io, mode)
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_17, GPIO_NUM_16, TWAI_MODE_NO_ACK);

    g_config.tx_queue_len = 10; // Set the transmit queue length
    g_config.rx_queue_len = 10; // Set the receive queue length

    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // Install TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK)
    {
        printf("TWAI driver install failed\n");
        return;
    }

    // Start TWAI driver
    if (twai_start() != ESP_OK)
    {
        printf("TWAI start failed\n");
        return;
    }

    // Receive the message
    for (size_t i = 0; i < 10; i++)
    {
        twai_message_t rx_msg;
        if (twai_receive(&rx_msg, pdMS_TO_TICKS(1000)) == ESP_OK)
        {
            printf("RX ID: 0x%03lX DLC: %d Data: ", rx_msg.identifier, rx_msg.data_length_code);
            for (int j = 0; j < rx_msg.data_length_code; j++)
            {
                printf("%02X ", rx_msg.data[j]);
            }
            printf("\n");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Stop and uninstall TWAI driver
    twai_stop();
    twai_driver_uninstall();
}
