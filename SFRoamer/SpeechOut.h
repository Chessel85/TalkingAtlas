//SpeechOut.h
#pragma once
#include <Tolk.h>
#include <string>

class CSpeechOut
{
//Constructor
public:
	CSpeechOut();
	~CSpeechOut();

//Methods
public:
	void SpeakString(const  std::string& rStr);
	void Repeat();
	std::string GetLastSpokenText();

//Variables
private:
	std::string m_LastSpoken;
};

