#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QTimer>
#include "mysquare.h"
#include <Box2D/Box2D.h>
#include "level.h"
#include "gamereader.h"
#include "patient.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    std::map<std::string, MySquare*> treatments; //string is name of the treatment and MySquare pointer is a pointer for the actual custom graphics view object to be added in model constructor
    std::pair<b2Body* , std::string> currentTreatment;
    Level* currentLevel;
    Patient *newPatient;
    int levelCount = 0;
    int wrongGuesses = 0;
    bool answeredIncorrectly = false;

    std::vector<std::string> hints;
    std::vector<std::string> neededTreatment;

    void setTreatmentCanDrop(std::string name, bool canDrop);
    void showHint();
    void collisionHelper(MySquare* caller);
    QLabel *levelPassed = new QLabel("QLabel text.");

signals:
    void fallOffShelf(int xLoc, int yLoc, std::string name);
    void showPopSignal();
    void loadUI();

public slots:
    void handleIncorrectAnswer();
    void collisionDetectionFromCaller(MySquare* caller);
    void loadLevel();
    void loadNextLevel();
    void EmitShowPop();


private:
std::vector<std::string> correctTreatmentOrder;
std::vector<pair<std::string, Level>> levelList;



};

#endif // MODEL_H
