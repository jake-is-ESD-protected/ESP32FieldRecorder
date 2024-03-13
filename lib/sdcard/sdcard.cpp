/**
 * @file sdcard.cpp
 * @author jake-is-ESD-protected
 * @date 2024-03-13
 * @brief SD card interfacing and tasks.
 *
 * 
 */

#include "sdcard.h"
#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "pin_defs.h"

static sdmmc_host_t host;
static sdspi_device_config_t slot_config;
static esp_vfs_fat_sdmmc_mount_config_t mount_config;
sdmmc_card_t* card;

e_err_t sd_init(void){
    host = SDSPI_HOST_DEFAULT();
    host.slot = SPI2_HOST;
    host.max_freq_khz = SDMMC_FREQ_DEFAULT;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_SPI_MOSI,
        .miso_io_num = PIN_SPI_MISO,
        .sclk_io_num = PIN_SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    if(spi_bus_initialize(host.slot, &bus_cfg, 1) != ESP_OK) { return e_err_bus_fault; }

    slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_SPI_SD_CS;
    slot_config.host_id = host.slot;

    mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
}

esp_err_t sd_mount(void){
    return esp_vfs_fat_sdspi_mount(SDCARD_BASE_PATH, &host, &slot_config, &mount_config, &card);
}

esp_err_t sd_unmount(void){
    return esp_vfs_fat_sdcard_unmount(SDCARD_BASE_PATH, card);
}

e_err_t sd_write(void* data, uint8_t type_size, uint32_t len, const char* fname) {
    FILE* f = fopen(fname, "wb");
    if (f == NULL) {
        return e_err_unavailable;
    }
    if(fwrite(data, type_size, len, f) != len) { 
        fclose(f);
        return e_err_file_io; 
    }
    fclose(f);
    return e_err_no_err;
}

e_err_t sd_read(void* data, uint8_t type_size, uint32_t len, const char* fname) {
    FILE* f = fopen(fname, "rb");
    if (f == NULL) {
        return e_err_unavailable;
    }
    if(fread(data, type_size, len, f) != len) { 
        fclose(f);
        return e_err_file_io; 
    }
    fclose(f);
    return e_err_no_err;
}
