#include <iostream>
#include <string>
#include "Configuration.h"

/**
 Date:    2021-06-25 20:17:00
 Author:  Lv Ning
 version: 0.1
 Description: This cpp file is used to load the configuration of the whole procedure.
*/
using namespace std;

Configuration loadConfiguration();
void showConfiguration(Configuration& configuration);


Configuration loadConfiguration()
{
	Configuration configuration = *new Configuration();
	showConfiguration(configuration);
	return configuration;
}
void showConfiguration(Configuration& configuration)
{
	cout << "The configuration is loaded!" << endl;
	cout << "**********************" << endl;
	cout << "  VRP Mode Configuration:" << endl;
	cout << configuration.toString();
	cout << "**********************" << endl;
}
