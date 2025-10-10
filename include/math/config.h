#pragma once

#include <Arduino_JSON.h>
#include <vector>

extern JSONVar config;

void lazyLoadConfig();
void loadConfig();
JSONVar parseCategory(String category,JSONVar &v);
String parseText(String name, JSONVar &v);
double parseFloat(String name, JSONVar &v);
int parseInt(String name, JSONVar &v);

struct RecursiveMultipleChoiceReturn{
    String type;
    JSONVar value;
};
RecursiveMultipleChoiceReturn parseRecursiveMultipleChoice(String name, JSONVar &v);
bool parseCheckbox(String name, JSONVar &v);
std::vector<JSONVar> parseList(String name, JSONVar &v);