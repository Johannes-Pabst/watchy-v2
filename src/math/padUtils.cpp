#include "math/padUtils.h"

String padLeft(String s, int length)
{
    while (s.length() < length)
    {
        s = "0" + s;
    }
    return s;
}