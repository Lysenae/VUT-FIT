/*IMS - Modelovani a simulace
 *
 * Project:  Simulator cernobilych stochastickych Petriho siti
 * File:     coreFunc.cpp
 * Author:   Dusan Kovacic, xkovac21, xkovac21@stud.fit.vutbr.cz
 *           Jaroslav Sendler, xsendl00, xsendl00@stud.fit.vutr.cz
 *
 * Encoding: UTF-8
 *
 * Description:
 */

#include "coreFunc.h"
#include "place.h"
#include "transition.h"
#include "directedArc.h"
#include "calendar.h"
#include <limits.h>
#include <stdio.h>
#include "stats.h"

std::vector<SCPlace*> g_allPlaces;
std::vector<SCTransition*> g_allTrans;
std::vector<SCDirectedArc*> g_allDirected;
SCCalendar g_eventCal;
double g_time = 0;
double g_simLength = 0;
unsigned int g_transIndex = 0;
unsigned int g_placeIndex = 0;
unsigned int g_arcIndex = 0;
bool g_print = true;
bool g_printT = true;
bool g_printAll = true;

int Run()
{
	if(!ValidateModel())
	{
		return -1;
	}
	unsigned int spawnPoints = 0;
	std::vector<SCTransition*>::iterator it;
	for(it = g_allTrans.begin(); it < g_allTrans.end(); it++)
	{
		if((*it)->GetDirectedArcsFrom()->empty())
		{
			g_eventCal.Insert(*it,(*it)->GetExactTime());
			spawnPoints++;
		}
	}
	if(g_simLength == 0 && spawnPoints != 0)
	{
		cout<<ERR_COMP_LENGHT<<endl;
		return -1;
	}
	else if(g_simLength == 0)
	{
		g_simLength = numeric_limits<double>::max();
	}
	if( g_print )
		SCStat::PrintLegend();

	int ret = 0;
	SCCalendarUnit *unit = NULL;
	while(g_time < g_simLength)
	{
		unit = g_eventCal.GetNextUnit();
		if(unit == NULL)
		{
			break;
		}
		else if(unit->GetTime() > g_simLength)
		{
			g_time = g_simLength;
			delete unit;
			break;
		}
		g_time = unit->GetTime();
		if((ret = unit->GetBase()->Run()) != BASE_OK)
		{
			if(SCBase::EvaluateErrorCode(ret,unit->GetBase()) == BASE_FATAL)
			{
				delete unit;
				break;
			}
		}
		delete unit;
	}

	if(g_printAll)												// tiskne souhrnou statistiku
		SCStat::PrintStatAll();

	cout << endl;
	return 0;
}
void SetSimulationLength(double length)
{
	g_simLength = length;
}
int PlToTr(SCPlace *start,SCTransition *end,SCDirectedArc *arc)
{
	start->AddDirectedArcTo(arc);
	end->AddDirectedArcFrom(arc);
	return 0;
}
int TrToPl(SCTransition *start,SCPlace *end,SCDirectedArc *arc)
{
	start->AddDirectedArcTo(arc);
	end->AddDirectedArcFrom(arc);
	return 0;
}

bool ValidateModel()
{
	bool ret = true;
	bool res = false;
	cout<<"PetriSim version 1.0.0"<<endl<<endl;
	cout<<"Checking for model validity... ";
	//preskuma ci sa v modely nenachadza nekonecny cyklus s nulovym casom
	vector<SCTransition*>::iterator it;
	vector<SCPlace*>::iterator it2;
	//prehliada vsetky miesta
	for(it2 = g_allPlaces.begin(); it2 < g_allPlaces.end(); it2++)
	{
		if(!(*it2)->CheckTransitions())
		{
			ret = false;
		}
	}
	//prehlada vsetky prechody
	for(it = g_allTrans.begin(); it < g_allTrans.end(); it++)
	{
		if((*it)->CheckZeroTimeLoop())
		{
			if(ret)
			{
				cout<<"failed!"<<endl<<endl;
			}
			cout<<ERR_COMP_LOOP<<endl;
			ret = false;
			break;
		}
	}
	if(ret)
	{
		cout<<"sucess!"<<endl<<endl;
	}
	return ret;
}
void PrintSteps( bool status )
{
	g_print = status;
}
void PrintSummary( bool status )
{
	g_printAll = status;
}
void ClearSimulation()
{
	g_allPlaces.clear();
	g_allTrans.clear();
	g_allDirected.clear();
	g_eventCal.Remove();
	g_time = 0;
	g_simLength = 0;
	g_transIndex = 0;
	g_placeIndex = 0;
	g_arcIndex = 0;
}
