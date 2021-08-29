#include "IInformation.h"
vector<FerryVehicleTask*> IInformation::nodes;
vector<FerryVehicleTask*> IInformation::depots;
vector<vector<double>> IInformation::matrix;
unordered_map<char, int> IInformation::flight2FerryVehcle;
vector<string> IInformation::index2flightName;
unordered_map<string, int> IInformation::flightName2index;
vector<Flight*> IInformation::flightTasks;
vector<vector<int>> IInformation::consequence;
vector<pair<int, int>> IInformation::positions;


