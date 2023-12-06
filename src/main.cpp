#include <Arduino.h>
#include "jescore.h"
#include "i2s_io.h"

void setup(){
    jes_init();

    register_job("i2sinit", I2S_TASK_INIT_MEM_SIZE, I2S_TASK_INIT_PRIORITY, i2s_init, false);
    register_job("i2srunin", I2S_TASK_RUN_MEM_SIZE, I2S_TASK_RUN_PRIORITY, i2s_run_in, true);
    register_job("i2srunout", I2S_TASK_RUN_MEM_SIZE, I2S_TASK_RUN_PRIORITY, i2s_run_out, true);

    launch_job("i2sinit");
    delay(100);
    launch_job("i2srunin");
    launch_job("i2srunout");
    while(1);
}

void loop(){

}