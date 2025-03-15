Starting from https://github.com/buglloc/esphome-components/tree/main/components/sy6970
esphome:
  name: t-display-s3-long
  friendly_name: LilyGo-T-Display-Long
  platformio_options:
    upload_speed: 921600
    build_unflags: -Werror=all
    board_build.flash_mode: dio

esp32:
  board: esp32-s3-devkitc-1
  flash_size: 16MB
  framework:
    type: esp-idf

external_components:
  - source: github://buglloc/esphome-components
    components: [ sy6970 ]

i2c:
  sda: 15
  scl: 10
  id: i2c_bus

# disable geen pmu state led (very annoying w/o battery)
sy6970:
  i2c_id: i2c_bus
  state_led_enable: false
