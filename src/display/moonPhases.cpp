#include "display/moonPhases.h"
#include "display/text.h"
#include "math/time.h"
#include "math/sky.h"
#include "math/vec3.h"
#include "images/moonPhases.h"
#include "hardware/display.h"
#include <Ephemeris.hpp>
#include "display/alignment.h"
#include <Arduino.h>

const int phaseCount=5;
RTC_SLOW_ATTR int phaseDays[phaseCount];
RTC_SLOW_ATTR int phaseIds[phaseCount];
RTC_SLOW_ATTR int lastDay;
RTC_SLOW_ATTR int lastMonth;
RTC_SLOW_ATTR int lastYear;

void clearMoonPhaseCache(){
    lastDay=0;
    lastMonth=0;
    lastYear=0;
    for(int i=0;i<phaseCount;i++){
        phaseDays[i]=0;
        phaseIds[i]=0;
    }
}
// 20 * 40
void moonPhasesWidget(int16_t x, int16_t y){
    /*
    phases:
        0: new
        1: growing
        2: full
        3: shrinking
    */
    SimpleTime today=simpleNow();
    if(lastDay!=today.day||lastMonth!=today.month||lastYear!=today.year){
        lastDay=today.day;
        lastMonth=today.month;
        lastYear=today.year;
        int phaseId=0;
        today.second=0;
        today.minute=0;
        today.hour=12;
        today.day+=1;
        fixTime(today);
        float lastLastScores[3]={};
        float lastScores[3]={};
        float phase=getMoonPhase(&today);
        for(int i=0;i<3;i++){
            lastLastScores[i]=getScore(phase,i);
        }
        today.day-=1;
        fixTime(today);
        phase=getMoonPhase(&today);
        for(int i=0;i<3;i++){
            lastScores[i]=getScore(phase,i);
        }
        while(true){
            today.day-=1;
            fixTime(today);
            phase=getMoonPhase(&today);
            for(int i=0;i<3;i++){
                float score=getScore(phase,i);
                if(score<lastScores[i]&&lastLastScores[i]<lastScores[i]){
                    today.day+=1;
                    fixTime(today);
                    phaseDays[phaseId]=today.day;
                    today.day-=1;
                    fixTime(today);
                    phaseIds[phaseId]=i;
                    if(i==1&&phase>0){
                        phaseIds[phaseId]+=2;
                    }
                    phaseId++;
                    if(phaseId>=1){
                        break;
                    }
                }
                lastLastScores[i]=lastScores[i];
                lastScores[i]=score;
            }
            if(phaseId>=1){
                break;
            }
        }
        today=simpleNow();
        today.second=0;
        today.minute=0;
        today.hour=12;
        phase=getMoonPhase(&today);
        for(int i=0;i<3;i++){
            lastLastScores[i]=getScore(phase,i);
        }
        today.day+=1;
        fixTime(today);
        phase=getMoonPhase(&today);
        for(int i=0;i<3;i++){
            lastScores[i]=getScore(phase,i);
        }
        while(true){
            today.day+=1;
            fixTime(today);
            phase=getMoonPhase(&today);
            for(int i=0;i<3;i++){
                float score=getScore(phase,i);
                if(score<lastScores[i]&&lastLastScores[i]<lastScores[i]){
                    today.day-=1;
                    fixTime(today);
                    phaseDays[phaseId]=today.day;
                    today.day+=1;
                    fixTime(today);
                    phaseIds[phaseId]=i;
                    if(i==1&&phase<0){
                        phaseIds[phaseId]+=2;
                    }
                    phaseId++;
                    if(phaseId>=phaseCount){
                        break;
                    }
                }
                lastLastScores[i]=lastScores[i];
                lastScores[i]=score;
            }
            if(phaseId>=phaseCount){
                break;
            }
        }
    }

    String days="";
    for(int i=0;i<phaseCount;i++){
        days+=String(phaseDays[i])+"\n";
        if(phaseIds[i]==0){
            dp.drawBitmap(x+12,y+8*i,epd_bitmap_new_moon,7, 7, fg_color);
        }
        if(phaseIds[i]==1){
            dp.drawBitmap(x+12,y+8*i,epd_bitmap_growing_moon,7, 7, fg_color);
        }
        if(phaseIds[i]==2){
            dp.drawBitmap(x+12,y+8*i,epd_bitmap_full_moon,7, 7, fg_color);
        }
        if(phaseIds[i]==3){
            dp.drawBitmap(x+12,y+8*i,epd_bitmap_shrinking_moon,7, 7, fg_color);
        }
    }
    textBox(days, x, y, 6*2, phaseCount*8, TD_TOP_RIGHT);
}
float getMoonPhase(SimpleTime *t){
    SolarSystemObject sun=calculatePos(Sun,t);
    SolarSystemObject moon=calculatePos(EarthsMoon,t);
    float moonx = cos(DEG_TO_RAD * moon.horiCoordinates.alt) * sin(DEG_TO_RAD * moon.horiCoordinates.azi);
    float moony = sin(DEG_TO_RAD * moon.horiCoordinates.alt);
    float moonz = cos(DEG_TO_RAD * moon.horiCoordinates.alt) * cos(DEG_TO_RAD * moon.horiCoordinates.azi);

    float sunx = cos(DEG_TO_RAD * sun.horiCoordinates.alt) * sin(DEG_TO_RAD * sun.horiCoordinates.azi);
    float suny = sin(DEG_TO_RAD * sun.horiCoordinates.alt);
    float sunz = cos(DEG_TO_RAD * sun.horiCoordinates.alt) * cos(DEG_TO_RAD * sun.horiCoordinates.azi);
    Vec3 vsun={-sunx, -suny, -sunz};
    Vec3 vmoon={moonx, moony, moonz};
    return vsun.dot(vmoon);
}
float getScore(float phase, int target){
    if(target==0){
        return -phase;
    }
    if(target==2){
        return phase;
    }
    if(target==1){
        return 1-abs(phase);
    }
    return 0;
}