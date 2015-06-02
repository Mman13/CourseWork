#ifndef EMOTIV_H
#define EMOTIV_H

#include <iostream>
#include <cassert>
#include <map>
#include <edk.h>

#pragma comment(lib, "../lib/edk.lib")

using namespace std;

class Affectiv
{
	public:
		Affectiv()
		{
			affectivMap[AFF_ENGAGEMENT_BOREDOM] = "Engagement";
			affectivMap[AFF_FRUSTRATION] = "Frustration";
			affectivMap[AFF_EXCITEMENT] = "Excitement";
			affectivMap[AFF_MEDITATION] = "Meditation";		
		}

		double rawScore;
		double minScale;
		double maxScale;
		double engagementBoredomScore;
		double frustrationScore;
		double shortTermExcitementScore;
		double longTermExcitementScore;
		double meditationScore;
		
		string emotion;

		double calculateScale (double& rawScore, double& maxScale, double& minScale)
		{
			double scaledScore;
			if (rawScore < minScale)
			{
				scaledScore = 0;
			}
			else if (rawScore > maxScale)
			{
				scaledScore = 1;
			}
			else
			{
				scaledScore = (rawScore - minScale) / (maxScale - minScale);
			}

			return scaledScore;
		}

	private:
		map<EE_AffectivAlgo_t, string> affectivMap;
};


class Expressiv
{
	public:
		Expressiv()
		{
			expressionMap[EXP_NEUTRAL] = "Neutral";
			expressionMap[EXP_BLINK] = "Blink";
			expressionMap[EXP_WINK_RIGHT] = "Wink Right";
			expressionMap[EXP_WINK_LEFT] = "Wink Left";
			expressionMap[EXP_SMILE] = "Smile";
			expressionMap[EXP_SMIRK_LEFT] = "Smirk Left";
			expressionMap[EXP_SMIRK_RIGHT] = "Smirk Right";
			expressionMap[EXP_LAUGH] = "Laugh";
			expressionMap[EXP_CLENCH] = "Clench";
			expressionMap[EXP_EYEBROW] = "Eyebrow Raise";
			expressionMap[EXP_FURROW] = "Eyebrow Furrow";
			expressionMap[EXP_HORIEYE] = "Horieye";
		}

		EE_ExpressivAlgo_t upperFaceType;
		EE_ExpressivAlgo_t lowerFaceType;
		float upperFaceAmp;
		float lowerFaceAmp;

		bool stringToExpression(const string& expStr, EE_ExpressivAlgo_t* exp)
		{
			assert(exp);
			map<EE_ExpressivAlgo_t, string>::const_iterator it;
			for (it = expressionMap.begin(); it != expressionMap.end(); it++)
			{
				if(it -> second == expStr)
				{
					*exp = it -> first;
					return true;
				}
			}
			return false;
		}

		string expressionToString(EE_ExpressivAlgo_t expType)
		{
			map<EE_ExpressivAlgo_t, string>::const_iterator it = expressionMap.find(expType);
			if ( it != expressionMap.end() ) return it -> second;
			else return "<unknown action>";
		}

	private:
		map<EE_ExpressivAlgo_t, string> expressionMap;
};

class Emotiv
{
	public:
		Emotiv();
		~Emotiv();
		void checkForNewEvent();

		// Affectiv
		void parseAffectiv(EmoStateHandle eState);

		//Expressiv
		void handleExpressivEvent(EmoEngineEventHandle expressivEvent);
		void handleExpressivCommands(const string& command, const unsigned int& ID, const string& expression, const unsigned int& sigType);
		void parseExpression(EmoStateHandle eState);

		//Gyro
		void gyro(unsigned int userID);

		string mode;

		Affectiv getAffectiv()
		{
			return aff;
		}

		Expressiv getExpressiv()
		{
			return exp;
		}

		EmoStateHandle getEstate()
		{
			return eState;
		}

	private:
		EmoStateHandle eState;
		EmoEngineEventHandle eEvent;

		//Gyro
		int *gyroX;
		int *gyroY;

		Affectiv aff;
		Expressiv exp;
};

#endif