#include "level.h"

Level::Level(string _title, vector<string> _stepsText, vector<string> _teachText, vector<string> _symptomsList, vector<string> _validTreatments,
              vector<pair<string, string>> _patientStagesImages, vector<pair<string, string>> _stepsImages, vector<pair<string, string>> _teachImages, vector<pair<string, string>> _medicineToStage, bool _inOrder)
{
    title = _title;
    stepsText = _stepsText;
    teachText = _teachText;
    symptomsList = _symptomsList;
    validTreatments = _validTreatments;
    patientStagesImages = _patientStagesImages;
    stepsImages = _stepsImages;
    teachImages = _teachImages;
    medicineToStage = _medicineToStage;
    inOrder = _inOrder;
}
