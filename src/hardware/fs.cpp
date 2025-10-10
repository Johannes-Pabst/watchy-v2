#include "hardware/fs.h"
#include <LittleFS.h>

void initFs(){
    LittleFS.begin();
}
bool removeDirRecursive(const char * dirname) {
    File dir = LittleFS.open(dirname);
    if (!dir) {
        return false;
    }
    if (!dir.isDirectory()) {
        return false;
    }

    File file = dir.openNextFile();
    while (file) {
        String path = String(dirname) + "/" + file.name();

        if (file.isDirectory()) {
            file.close();
            if (!removeDirRecursive(path.c_str())) {
                return false;
            }
        } else {
            file.close();
            if (!LittleFS.remove(path)) {
                return false;
            }
        }
        file = dir.openNextFile();
    }

    return LittleFS.rmdir(dirname);
}