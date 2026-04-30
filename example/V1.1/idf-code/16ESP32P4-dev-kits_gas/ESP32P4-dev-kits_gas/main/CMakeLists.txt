FILE(GLOB_RECURSE main ${CMAKE_SOURCE_DIR}/main/*.c)

idf_component_register(SRCS ${main}
                        INCLUDE_DIRS "include" 
                        REQUIRES esp_timer bsp_led bsp_gas)
                                 


 