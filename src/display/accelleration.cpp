#include "display/accelleration.h"
#include "hardware/bma.h"
#include "display/text.h"
#include "display/alignment.h"

void accellerationWidget(int16_t x, int16_t y){
    Accel acc;
    getAcc(acc);
    String s="X:"+String(acc.x)+"\nY:"+String(acc.y)+"\nZ:"+String(acc.z);
    textBox(s,x,y,6*7,8*3,TD_TOP_LEFT,__null,1);
}