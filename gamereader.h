#ifndef GAMEREADER_H
#define GAMEREADER_H

#include <iostream>
#include <vector>
#include "level.h"
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

using namespace std;

class GameReader
{
public:
    GameReader(string = "");
    vector<pair<string, Level>> getLevels();

private:
    vector<pair<string, Level>> levelData;
    void readFile(QFile *file);
};

//Comment

#endif // GAMEREADER_H
