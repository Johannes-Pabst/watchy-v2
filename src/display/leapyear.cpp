#include "display/leapyear.h"
#include "hardware/display.h"
#include "images/dino.h"
#include "math/time.h"

// Hash / mix a 32-bit integer. (You can use 32 or 64 bit depending on your platform.)
static uint32_t mix_uint32(uint32_t x) {
    // A simple bit‐scrambler (variant of “xorshift / bit mix”)
    x ^= x >> 16;
    x *= 0x7feb352d;  // a random odd constant
    x ^= x >> 15;
    x *= 0x846ca68b;  // another odd constant
    x ^= x >> 16;
    return x;
}

// Map an integer (year) to float in [0,1)
float year_to_uniform_float(int year) {
    // Cast / treat as unsigned to avoid UB on negative shifts
    uint32_t h = mix_uint32((uint32_t)year);
    // Normalize: divide by max + 1 (so result < 1.0)
    // Since h is 32 bit, we divide by 2^32 as float.
    // But we avoid overflow: convert to double (or float) first.
    const double inv = 1.0 / 4294967296.0;  // 2^32
    return (float)(h * inv);
}
// 36 * 24
void leapyearWidget(int16_t x, int16_t y){
    SimpleTime t=simpleNow();
    if(isLeapYear(t.year)){
        dp.drawBitmap(x, y, epd_bitmap_dino, 9, 11, fg_color);
    }else{
        dp.drawBitmap(x, y+12, epd_bitmap_dino, 9, 11, fg_color);
    }
    for(int i=0;i<4;i++){
        if(isLeapYear(t.year+i)){
            dp.drawBitmap(x+i*9, y+12, epd_bitmap_cactus, 9, 11, fg_color);
        }else if(i>0){
            dp.drawBitmap(x+i*9, y+12, epd_bitmap_paths[int(year_to_uniform_float(t.year+i)*4)], 9, 11, fg_color);
        }
    }
}