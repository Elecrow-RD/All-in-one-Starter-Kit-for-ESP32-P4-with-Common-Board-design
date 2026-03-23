set(ELECROW_DIR ./)
file(GLOB_RECURSE ELECROW_C_SRCS ${ELECROW_DIR}/*.c)

idf_component_register(
    SRCS  ${ELECROW_C_SRCS}
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "priv_include"
    REQUIRES driver
    PRIV_REQUIRES esp_lcd usb spiffs fatfs 
)
