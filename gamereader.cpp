#include "gamereader.h"

GameReader::GameReader(string filename)
{
    QFile *file = new QFile(QString::fromStdString(filename));

    if(file->exists()) {
        readFile(file);
    }
    else
        cout << "Could not find level data." << endl;

}

vector<pair<string, Level>> GameReader::getLevels()
{
    return levelData;
};

void GameReader::readFile(QFile *file)
{
    if( file->open( QIODevice::ReadOnly ) )
    {
        QByteArray bytes = file->readAll();
        file->close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson( bytes, &jsonError );
        if( jsonError.error != QJsonParseError::NoError )
        {
            std::cout << "fromJson failed: " << jsonError.errorString().toStdString() << std::endl;
            return ;
        }
        if( document.isArray() )
        {
            QJsonArray arr = document.array();
            foreach(const QJsonValueRef obj, arr)
            {
                string title;
                vector<string> stepsText; // Text if you get it wrong: {"First you have to clean the wound using hydrogen peroxide.", "Second you have to put on antibiotic get because..", ...}
                vector<string> teachText;  // Text (serperate by new lines): {"Pain Med Education", "Ibuprofen: typically used for feverish pain.", ... }
                vector<string> symptomsList; // Text of symtoms: {"Pain", "Trouble Breathing", "Fever"}
                vector<string> validTreatments; //ordered - based of inOrder : {"hot-pack", "ibuprofen", "band-aid", ..}

                // Image Data
                vector<pair<string, string>> patientStagesImages; // SP1 (sprained ankle) , QPixmap of SP1 image
                vector<pair<string, string>> stepsImages; // Medicine name, resource path of medicine
                vector<pair<string, string>> teachImages; // Images to use on popup to teach subject (optional): Medicine name, resource path of medicine

                // Object Data
                vector<pair<string, string>> medicineToStage; // Medicine name, stage - SP1 (sprained ankle stage 1)
                bool inOrder;

                QJsonObject readObj = obj.toObject();

                // Text Data
                if (readObj.contains("title"))
                    title = readObj.value("title").toString().toStdString();

                if (readObj.contains("stepsText")){
                    QJsonArray st = readObj.value("stepsText").toArray();
                    foreach (const QJsonValueRef s, st){
                        stepsText.push_back(s.toString().toStdString());
                    }
                }

                if (readObj.contains("teachText")){
                    QJsonArray tt = readObj.value("teachText").toArray();
                    foreach (const QJsonValueRef t, tt){
                        teachText.push_back(t.toString().toStdString());
                    }
                }

                if (readObj.contains("stepsText")){
                    QJsonArray st = readObj.value("stepsText").toArray();
                    foreach (const QJsonValueRef s, st){
                        stepsText.push_back(s.toString().toStdString());
                    }
                }

                if (readObj.contains("symptomsList")){
                    QJsonArray st = readObj.value("symptomsList").toArray();
                    foreach (const QJsonValueRef s, st){
                        symptomsList.push_back(s.toString().toStdString());
                    }
                }

                if (readObj.contains("validTreatments")){
                    QJsonArray st = readObj.value("validTreatments").toArray();
                    foreach (const QJsonValueRef s, st){
                        validTreatments.push_back(s.toString().toStdString());
                    }
                }

                // Image Data
                if (readObj.contains("patientStagesImages")){
                    QJsonArray st = readObj.value("patientStagesImages").toArray();
                    foreach (const QJsonValueRef s, st){
                        QJsonArray arr = s.toArray();
                        patientStagesImages.push_back(pair<string, string>(arr[0].toString().toStdString(), arr[1].toString().toStdString()));
                    }
                }

                if (readObj.contains("stepsImages")){
                    QJsonArray st = readObj.value("stepsImages").toArray();
                    foreach (const QJsonValueRef s, st){
                        QJsonArray arr = s.toArray();
                        stepsImages.push_back(pair<string, string>(arr[0].toString().toStdString(), arr[1].toString().toStdString()));
                    }
                }

                if (readObj.contains("teachImages")){
                    QJsonArray st = readObj.value("teachImages").toArray();
                    foreach (const QJsonValueRef s, st){
                        QJsonArray arr = s.toArray();
                        teachImages.push_back(pair<string, string>(arr[0].toString().toStdString(), arr[1].toString().toStdString()));
                    }
                }

                if (readObj.contains("medicineToStage")){
                    QJsonArray st = readObj.value("medicineToStage").toArray();
                    foreach (const QJsonValueRef s, st){
                        QJsonArray arr = s.toArray();
                        medicineToStage.push_back(pair<string, string>(arr[0].toString().toStdString(), arr[1].toString().toStdString()));
                    }
                }

                // Bool
                if (readObj.contains("inOrder"))
                    inOrder = readObj.value("inOrder").toBool();

                Level *newLev = new Level(title, stepsText, teachText, symptomsList, validTreatments, patientStagesImages, stepsImages, teachImages, medicineToStage, inOrder);

                levelData.push_back(pair<string, Level>(title, *newLev));
            }
        }
    }
};
