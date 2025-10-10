#include "display/text.h"
#include "hardware/display.h"
#include <Display.h>
#include <GxEPD2_BW.h>
#include <Arduino.h>
#include <vector>
#include "display/text.h"
#include "display/alignment.h"

struct Line{
    String text;
    uint8_t width;
};
bool textBox(String text, int16_t x, int16_t y, int16_t width, int16_t height, u_int8_t td, const GFXfont *f, uint8_t fontSize, uint16_t color, int *outw, int *outh)
{
    dp.setTextColor(color);
    dp.setFont(f);
    dp.setTextSize(fontSize);
    int sid = 0;
    int eid = 0;
    std::vector<Line> lines;
    String line;
    uint8_t textsize_x=0;
    uint8_t linesize_x = 0;
    uint8_t textsize_y = 0;
    while (true)
    {
        if (eid >= text.length()||text[eid] == '\n')
        {
            lines.push_back({line, linesize_x});
            textsize_x=max(textsize_x, linesize_x);
            linesize_x = 0;
            textsize_y += getYAdvance(f) * fontSize;
            line = "";
            sid = eid + 1;
            eid = sid;
            if (eid >= text.length())
            {
                break;
            }
            else
            {
                continue;
            }
        }
        char c = text[eid];
        linesize_x += getXAdvance(f, c) * fontSize;
        if (linesize_x > width)
        {
            lines.push_back({line, linesize_x});
            textsize_x=max(textsize_x, linesize_x);
            line = c;
            linesize_x = getXAdvance(f, c) * fontSize;
            textsize_y += getYAdvance(f) * fontSize;
            sid = eid + 1;
            eid = sid;
        }
        else
        {
            line += c;
            eid++;
        }
    }
    uint8_t x_dir;
    uint8_t y_dir;
    splitAlignment(td, x_dir, y_dir);
    int16_t y_overflow = height - textsize_y;
    int16_t y_offset;
    int16_t y_offset_2=0;
    if (y_dir == TD1_START)
    {
        y_offset = 0;
    }else if (y_dir == TD1_CENTER)
    {
        y_offset = y_overflow / 2;
    }else if (y_dir == TD1_END)
    {
        y_offset = y_overflow;
    }
    if(f!=nullptr){
        y_offset_2+=(f->yAdvance-1)*fontSize;
    }
    for (int i = 0; i < lines.size(); i++)
    {
        if (y_offset + getYAdvance(f) * fontSize > height)
        {
            return false;
        }
        Line line = lines[i];
        int16_t x_overflow = width - line.width;
        int16_t x_offset;
        if (x_dir == TD1_START)
        {
            x_offset = 0;
        }
        else if (x_dir == TD1_CENTER){
            x_offset = x_overflow / 2;
        }
        else if (x_dir == TD1_END){
            x_offset = x_overflow;
        }
        dp.setCursor(x + x_offset, y + y_offset+y_offset_2);
        dp.print(line.text);
        y_offset += getYAdvance(f) * fontSize;
    }
    if(outw!=nullptr){
        *outw=int(textsize_x);
    }
    if(outh!=nullptr){
        *outh=int(textsize_y);
    }
    return true;
}
uint8_t getYAdvance(const GFXfont *f){
    if (f == nullptr)
    {
        return 8;
    }
    return f->yAdvance;
}
uint8_t getXAdvance(const GFXfont *f, char c)
{
    if (f == nullptr || f->glyph == nullptr)
        return 6;

    int16_t index = c - f->first;
    if (index < 0 || index >= (f->last - f->first + 1))
        return 6;

    return f->glyph[index].xAdvance;
}