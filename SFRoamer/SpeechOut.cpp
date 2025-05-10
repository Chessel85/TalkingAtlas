//SpeechOut.cpp
#include "pch.h"
#include <string>
#include <locale>
#include <codecvt>
#include "SpeechOut.h"

CSpeechOut::CSpeechOut()
{
	//Initialise tolk
	Tolk_Load();
}

CSpeechOut::~CSpeechOut()
{
	//Release tolk 
	Tolk_Unload();
}

void CSpeechOut::SpeakString(const  std::string& rStr)
{
	std::wstring wStr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(rStr);
	Tolk_Output(wStr.c_str());
	m_LastSpoken = rStr;
}

void CSpeechOut::Repeat()
{
	SpeakString(m_LastSpoken);
}

std::string CSpeechOut::GetLastSpokenText()
{
	return m_LastSpoken;
}