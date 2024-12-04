#include "cJSON.h"
#include "driver/i2c_master.h"
#include <stdio.h>

#define DLHR_I2C_PORT 0
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 23

void app_main(void) {
  i2c_master_bus_config_t i2c_mst_config = {
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .i2c_port = DLHR_I2C_PORT,
      .scl_io_num = I2C_MASTER_SCL_IO,
      .sda_io_num = I2C_MASTER_SDA_IO,
      .glitch_ignore_cnt = 7,
      .flags.enable_internal_pullup = true,
  };

  i2c_master_bus_handle_t bus_handle;
  ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

  i2c_device_config_t dlhr_cfg = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = 0x58,
      .scl_speed_hz = 100000,
  };

  i2c_master_dev_handle_t dlhr_handle;

  ESP_ERROR_CHECK(
      i2c_master_bus_add_device(bus_handle, &dlhr_cfg, &dlhr_handle));

  uint8_t command[1] = {0xAA};
  uint8_t response_buf[8];

  ESP_ERROR_CHECK(i2c_master_transmit_receive(
      dlhr_handle, command, sizeof(command), response_buf, 8, -1));

  cJSON *root, *fmt;
  root = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "pressures", "0.000");
  cJSON_Print(root);
}
