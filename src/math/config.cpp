#include "math/config.h"
#include <Arduino_JSON.h>
#include <LittleFS.h>
#include <vector>

JSONVar config;
bool config_initialized=false;

void lazyLoadConfig(){
    if(!config_initialized){
        loadConfig();
    }
}

void loadConfig()
{
    if (LittleFS.exists("/config.json"))
    {
        File file = LittleFS.open("/config.json", "r");
        String content = file.readString();
        file.close();
        config = JSON.parse(content);
        if (JSON.typeof(config) == "null")
        {
            config = JSON.parse("{}");
        }
    }
    else
    {
        config = JSON.parse("{}");
    }
    config_initialized=true;
}

JSONVar parseCategory(String category,JSONVar &v)
{
    if (JSON.typeof(v[category]) == "null")
    {
        v[category] = JSON.parse("{}");
    }
    return v[category];
}
String parseText(String name, JSONVar &v){
    if (JSON.typeof(v[name]) == "null")
    {
        v[name] = "";
    }
    return (const char*)v[name];
}
double parseFloat(String name, JSONVar &v){
    if (JSON.typeof(v[name]) == "null")
    {
        v[name] = 0.0;
    }
    return (double)v[name];
}
int parseInt(String name, JSONVar &v){
    if (JSON.typeof(v[name]) == "null")
    {
        v[name] = 0;
    }
    return (int)v[name];
}
RecursiveMultipleChoiceReturn parseRecursiveMultipleChoice(String name, JSONVar &v){
    RecursiveMultipleChoiceReturn r;
    if (JSON.typeof(v[name]) == "null")
    {
        v[name] = JSON.parse("{}");
        v[name]["type"]="choice";
    }
    String type=(const char*)v[name]["type"];
    r.type=type;
    r.value=v[name];
    return r;
}
bool parseCheckbox(String name, JSONVar &v){
    if (JSON.typeof(v[name]) == "null")
    {
        v[name]=false;
    }
    return (bool)v[name];
}
std::vector<JSONVar> parseList(String name, JSONVar &v){
    std::vector<JSONVar> r;
    if (JSON.typeof(v[name]) == "null")
    {
        v[name]=JSON.parse("[]");
    }
    JSONVar list=v[name];
    int len=list.length();
    for(int i=0;i<len;i++){
        r.push_back(list[i]);
    }
    return r;
}