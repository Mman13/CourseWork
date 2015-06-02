#include "Emotiv.h"

Emotiv::Emotiv()
{
	eEvent = EE_EmoEngineEventCreate();
	eState = EE_EmoStateCreate();

	mode = "headset";

	if (mode == "Headset" || mode == "headset")
	{
		if (EE_EngineConnect() != EDK_OK)
		{
			throw std::exception("Failed to Connect to Headset, attempting Composer connection");
		}
		else
		{
			//cout << "Successfully connected to Headset" << endl;
			checkForNewEvent();
		}
	}

	if (mode == "Composer" || mode == "composer")
	{
		if (EE_EngineRemoteConnect("127.0.0.1", 1726) != EDK_OK)
		{
			throw std::exception("Failed to connect to EmoComposer");
		}
		else
		{
			cout << "Successfully connected to EmoComposer" << endl;
			checkForNewEvent();
		}
	}

	aff.minScale = 0.0f;
	aff.maxScale = 0.0f;
	aff.rawScore = 0.0f;
	aff.engagementBoredomScore = 0.0f;
	aff.frustrationScore = 0.0f;
	aff.shortTermExcitementScore = 0.0f;
	aff.longTermExcitementScore = 0.0f;
	aff.meditationScore = 0.0f;
	exp.lowerFaceAmp = 0.0f;
	exp.upperFaceAmp = 0.0f;
	gyroX = 0;
	gyroY = 0;
}

std::string numberToString(const int n)
{
	char buf[80];
	_itoa_s(n, buf, 10);
	return std::string(buf);
}

void Emotiv::checkForNewEvent()
{
	try 
	{
		int state = EE_EngineGetNextEvent(eEvent);
		unsigned int userID = 0;

		//New event needs to be handled
		if (state == EDK_OK)
		{
			//Get Event Properties
			EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
			EE_EmoEngineEventGetUserId(eEvent, &userID);

			switch (eventType)
			{
			case EE_UserAdded: //New headset connected
				cout << "New user " << userID << " has been added." << endl;
				break;
			case EE_UserRemoved: //Headset disconnected
				cout << "User " << userID << " has been removed." << endl;
				break;
			case EE_EmoStateUpdated: //EmoState has been updated
				EE_EmoEngineEventGetEmoState(eEvent, eState);
				parseAffectiv(eState);
				parseExpression(eState);
				break;
			case EE_ExpressivEvent: //Expressiv training event
				handleExpressivEvent(eEvent);
				break;
			default:
				break;
			}
		}
	}
	catch (const exception& e)
	{
		cerr << e.what() << endl;
	}
}

void Emotiv::parseAffectiv(EmoStateHandle eState)
{
	ES_AffectivGetEngagementBoredomModelParams(eState, &aff.rawScore, &aff.minScale, &aff.maxScale);
	//cout << "Engagement/Bored raw score: " << &aff.rawScore << ", min scale: " << aff.minScale << ", max scale: " << aff.maxScale;
	if (aff.minScale == aff.maxScale)
	{
		//cout << ", scaled score: undefined" << endl;
	}
	else
	{
		aff.engagementBoredomScore = aff.calculateScale(aff.rawScore, aff.minScale, aff.maxScale);
		//cout << ", scaled score: " << aff.engagementBoredomScore << endl;
	}

	ES_AffectivGetFrustrationModelParams(eState, &aff.rawScore, &aff.minScale, &aff.maxScale);
	//cout << "Frustration raw score: " << &aff.rawScore << ", min Scale: " << aff.minScale << ", max scale: " << aff.maxScale;
	if (aff.minScale == aff.maxScale)
	{
		//cout << ", scaled score: undefined" << endl;
	}
	else
	{
		aff.frustrationScore = aff.calculateScale(aff.rawScore, aff.minScale, aff.maxScale);
		//cout << ", scaled score: " << aff.frustrationScore << endl;
	}

	ES_AffectivGetMeditationModelParams(eState, &aff.rawScore, &aff.minScale, &aff.maxScale);
	//cout << "Meditation raw score: " << &aff.rawScore << ", min scale: " << aff.minScale << ", max scale: " << aff.maxScale;
	if (aff.minScale == aff.maxScale)
	{
		//cout << ", scaled score: undefined" << endl;
	}
	else
	{
		aff.meditationScore = aff.calculateScale(aff.rawScore, aff.minScale, aff.maxScale);
		//cout << ", scaled score: " << aff.meditationScore << endl;
	}

	ES_AffectivGetExcitementShortTermModelParams(eState, &aff.rawScore, &aff.minScale, &aff.maxScale);
	//cout << "Short Term Excitement raw score: " << &aff.rawScore << ", min scale: " << aff.minScale << ", max scale: " << aff.maxScale;
	if (aff.minScale == aff.maxScale)
	{
		//cout << ", scaled score: undefined" << endl;
	}
	else
	{
		aff.shortTermExcitementScore = aff.calculateScale(aff.rawScore, aff.minScale, aff.maxScale);
		//cout << ", scaled score: " << aff.shortTermExcitementScore << endl;
	}

	aff.engagementBoredomScore = ES_AffectivGetEngagementBoredomScore(eState);
	aff.frustrationScore = ES_AffectivGetFrustrationScore(eState);
	aff.meditationScore = ES_AffectivGetMeditationScore(eState);
	aff.shortTermExcitementScore = ES_AffectivGetExcitementShortTermScore(eState);
	aff.longTermExcitementScore = ES_AffectivGetExcitementLongTermScore(eState);

	if (aff.engagementBoredomScore < 0.5)
	{
		aff.emotion = "Bored";
	}

	if (aff.engagementBoredomScore >= 0.5)
	{
		aff.emotion = "Engaged";
	}

	if (aff.frustrationScore >= 0.5)
	{
		aff.emotion = "Frustrated";
	}

	if (aff.meditationScore >= 0.5)
	{
		aff.emotion = "Meditation";
	}
	
	if (aff.shortTermExcitementScore >= 0.5)
	{
		aff.emotion = "Excited";
	}

	if (aff.engagementBoredomScore < 0.5 && 
		aff.frustrationScore < 0.5 && 
		aff.meditationScore < 0.5 && 
		aff.shortTermExcitementScore < 0.5) 
	{
		aff.emotion = "neutral";
	}

	/*aff.engagementBoredomScore = 0.5;
	aff.frustrationScore  = 0.5*/

	/*cout << "Engagement/boredom score: " << aff.engagementBoredomScore << endl;
	cout << "Frustration score: " << aff.frustrationScore << endl;
	cout << "Meditation score: " << aff.meditationScore << endl;
	cout << "Short Term Excitement score: " << aff.shortTermExcitementScore << endl;
	cout << "Long Term Excitement score: " << aff.longTermExcitementScore << endl;
	cout << endl;*/
}

void Emotiv::handleExpressivEvent(EmoEngineEventHandle expressivEvent)
{
	unsigned int userID = 0;

	//Get Event Properties
	EE_ExpressivEvent_t eventType = EE_ExpressivEventGetType(expressivEvent);
	EE_EmoEngineEventGetUserId(expressivEvent, &userID);

	switch (eventType)
	{
	case EE_ExpressivTrainingStarted: //Expressive Training has started
		cout << "Expressiv training for user " << userID << " has started." << endl;
		break;
	case EE_ExpressivTrainingSucceeded: //Expressive training has succeeded
		cout << "Expressiv training for user " << userID << " has succeeded." << endl;
		break;
	case EE_ExpressivTrainingFailed: //Expressive training has failed
		cout << "Expressiv training for user " << userID << " has failed." << endl;
		break;
	case EE_ExpressivTrainingCompleted: //Expressive training has completed
		cout << "Expressiv training for user " << userID << " has completed." << endl;
		break;
	case EE_ExpressivTrainingDataErased: //Expressive training has been erased
		cout << "Expressiv training data for user " << userID << " has been erased." << endl;
		break;
	case EE_ExpressivTrainingRejected: //Expressive training has been rejected
		cout << "Expressiv training for user " << userID << " has been rejected." << endl;
		break;
	case EE_ExpressivTrainingReset: //Expressive training has been reset
		cout << "Expressiv training for user " << userID << " has been reset." << endl;
		break;
	case EE_ExpressivNoEvent:
	default:
		//Unhandled Case
		assert(0);
		return;
	}
}

void Emotiv::handleExpressivCommands(const string& command, const unsigned int& ID, const string& expression, const unsigned int& sigType) {
	bool wrongArgument = true;

	if (command == "query_trained_signature") {	//Query for activating a trained signature
		int signatureAvailable = 0;
		unsigned int userID = ID;
		cout << "Querying availability of a trained Expressiv signature for user " << userID << endl;
		wrongArgument = (EE_ExpressivGetTrainedSignatureAvailable(userID, &signatureAvailable) != EDK_OK);
		if (!wrongArgument) {
			cout << "A trained Expressiv signature is " << ((!signatureAvailable) ? "not" : "") << " available for user " << userID;
		}
	}

	if (command == "set_signature") {	//Switch between the universal signature or a trained signature
		unsigned int userID = ID, signatureType = sigType;
		int result;
		if (signatureType >= (unsigned int)EXP_SIG_UNIVERSAL && signatureType <= (unsigned int)EXP_SIG_TRAINED) {
			cout << "Switching to " << ((signatureType) ? "a trained" : "the universal") << " Expressiv signature for user " << userID << endl;
			result = EE_ExpressivSetSignatureType(userID,(EE_ExpressivSignature_t)signatureType);
            wrongArgument = (result != EDK_OK && result != EDK_EXP_NO_SIG_AVAILABLE);
            if ( result == EDK_EXP_NO_SIG_AVAILABLE ) {
                cout << "A trained Expressiv signature is not available for user " << userID << endl;
            }
		}
	}

	if (command == "train_expressiv_action") {	//Change Expressiv training action
		unsigned int userID = ID;
		EE_ExpressivAlgo_t expType;
		const string expStr = expression;
		if (exp.stringToExpression(expStr, &expType)) {
			cout << "Setting Expressiv training expression for user " << userID << " to " << expStr << endl;
			wrongArgument = (EE_ExpressivSetTrainingAction(userID, expType) != EDK_OK);
		} else {
			cout << "Expression [" << expStr << "] cannot be trained.";
		}
	}

	if (command == "start_expressiv_training") {	//Start Expressiv training
		unsigned int userID = ID;
		cout << "Start Expressiv training for user " << userID << endl;
		wrongArgument = (EE_ExpressivSetTrainingControl(userID, EXP_START) != EDK_OK);
	}

	if (command == "accept_expressiv_training") {	//Accept Expressiv training
		unsigned int userID = ID;
		cout << "Accepting Expressiv training for user " << userID << endl;
		wrongArgument = (EE_ExpressivSetTrainingControl(userID, EXP_ACCEPT) != EDK_OK);
	}

	if (command == "reject_expressiv_training") {	//Reject Expressiv training
		unsigned int userID = ID;
		cout << "Rejecting Expressiv training for user " << userID << endl;
		wrongArgument = (EE_ExpressivSetTrainingControl(userID, EXP_REJECT) != EDK_OK);
	}

	if (command == "erase_expressiv_training") {	//Erase training data
		unsigned int userID = ID;
		EE_ExpressivAlgo_t expType;
		if (EE_ExpressivGetTrainingAction(userID, &expType) == EDK_OK) {
			cout << "Erasing Expressiv training for action " << exp.expressionToString(expType) << " for user " << userID << endl;
			wrongArgument = (EE_ExpressivSetTrainingControl(userID, EXP_ERASE) != EDK_OK);
		}
	}
}

void Emotiv::parseExpression(EmoStateHandle eState) {
	exp.upperFaceType = ES_ExpressivGetUpperFaceAction(eState);
	exp.lowerFaceType = ES_ExpressivGetLowerFaceAction(eState);
	exp.upperFaceAmp = ES_ExpressivGetUpperFaceActionPower(eState);
	exp.lowerFaceAmp = ES_ExpressivGetLowerFaceActionPower(eState);

	if (ES_ExpressivIsBlink(eState))
		cout << "Blink Detected" << endl;
	if (ES_ExpressivIsLeftWink(eState))
		cout << "Left Wink Detected" << endl;
	if (ES_ExpressivIsRightWink(eState))
		cout << "Right Wink Detected" << endl;
	if (ES_ExpressivIsEyesOpen(eState))
		cout << "Eyes are open" << endl; 
	else
		cout << "Eyes are closed" << endl;
	if (ES_ExpressivIsLookingUp(eState))
		cout << "Looking Up Detected" << endl;
	if (ES_ExpressivIsLookingDown(eState))
		cout << "Looking Down Detected" << endl;
	if (ES_ExpressivIsLookingRight(eState))
		cout << "Looking Right Detected" << endl;
	if (ES_ExpressivIsLookingLeft(eState))
		cout << "Looking Left Detected" << endl;
	if (exp.upperFaceAmp > 0.0) 
	{
		switch (exp.upperFaceType) 
		{
			case EXP_EYEBROW:
				cout << "Raised Eyebrows Detected with " << numberToString(static_cast<int>(exp.upperFaceAmp*100.0f)) << " amplitude." << endl;
				break;
			case EXP_FURROW:
				cout << "Furrowed Eyebrows Detected with " << numberToString(static_cast<int>(exp.upperFaceAmp*100.0f)) << " amplitude." << endl;
				break;
			default:
				break;
		}
	}
	if (exp.lowerFaceAmp > 0.0) 
	{
		switch (exp.lowerFaceType) 
		{
			case EXP_CLENCH:
				cout << "Teeth Clenching Detected with " << numberToString(static_cast<int>(exp.lowerFaceAmp*100.0f)) << " amplitude." << endl;
				break;
			case EXP_SMILE:
				cout << "Smile Detected with " << numberToString(static_cast<int>(exp.lowerFaceAmp*100.0f)) << " amplitude." << endl;
				break;
			case EXP_LAUGH:
				cout << "Laughter Detected" << endl;
				break;
			case EXP_SMIRK_LEFT:
				cout << "Left Smirking Detected" << endl;
				break;
			case EXP_SMIRK_RIGHT:
				cout << "Right Smirking Detected" << endl;
				break;
			default:
				break;
		}
	}

	cout << endl;
}

void Emotiv::gyro(unsigned int userID)
{
	EE_HeadsetGyroRezero(userID);
	EE_HeadsetGetGyroDelta(userID, gyroX, gyroY);
}

//Deconstructor
Emotiv::~Emotiv()
{
	EE_EngineDisconnect();
	EE_EmoStateFree(eState);
	EE_EmoEngineEventFree(eEvent);
}