#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <esp_wifi.h>
#include <esp_netif.h>
#include "esp_log.h"
#include "wifi_manager.h"

/* @brief tag used for ESP serial console messages */
static const char TAG[] = "main";

/**
 * @brief RTOS task that periodically prints the heap memory available.
 * @note Pure debug information, should not be ever started on production code! This is an example on how you can integrate your code with wifi-manager
 */
/*
void monitoring_task(void *pvParameter)
{
	for(;;){
		ESP_LOGI(TAG, "free heap: %d",esp_get_free_heap_size());
		vTaskDelay( pdMS_TO_TICKS(10000) );
	}
}
*/

void cb_connection_ok(void *pvParameter){
	ip_event_got_ip_t* param = (ip_event_got_ip_t*)pvParameter;

	/* transform IP to human readable string */
	char str_ip[16];
	esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, IP4ADDR_STRLEN_MAX);

	ESP_LOGI(TAG, "I have a connection and my IP is %s!", str_ip);
}

void app_main(void)
{
    /*
    printf("Hello world!\n");

    // Print chip information 
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
    */

    /* start the wifi manager */
	wifi_manager_start();

	/* register a callback as an example to how you can integrate your code with the wifi manager */
	wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);

    /* your code should go here. Here we simply create a task on core 2 that monitors free heap memory */
	//xTaskCreatePinnedToCore(&monitoring_task, "monitoring_task", 2048, NULL, 1, NULL, 1);
}