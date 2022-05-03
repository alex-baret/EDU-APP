#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include <QPixmap>

using namespace std;


class Level
{
public:
    Level(string _title, vector<string> _stepsText, vector<string> _teachText, vector<string> _symptomsList, vector<string> _validTreatments,
           vector<pair<string, string>> _patientStagesImages, vector<pair<string, string>> _stepsImages, vector<pair<string, string>> _teachImages, vector<pair<string, string>> _medicineToStage, bool _inOrder);

    // Text Data
    string title;
    // The idea for stepsText is a popup that displays the image (stepsImages) of the medicine followed by the respective explanation stored in this.
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
};

// Sprained Ankle - SP1 SP2 SP3
// Headache - HA1, HA2, HA3

#endif // LEVEL_H
