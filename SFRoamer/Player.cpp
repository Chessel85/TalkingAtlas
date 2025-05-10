//Player.cpp
#include "pch.h"
#include "Player.h"
#include <math.h>

const double DEFAULT_STEP_SIZE = 50.0;
const double MAX_STEP_SIZE = 1000;
const double MAX_LATITUDE = 88;
const double MIN_LATITUDE = -88;
const double M_PI = 3.141592653589793;

CPlayer::CPlayer() : m_Steps{ 0.001, 0.005, 0.01, 0.05, 0.1, 0.5, 1,  2, 3, 4, 5, 10, 25, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 }
{
	m_StepSizeIndex = 14;
	//m_StepSize = DEFAULT_STEP_SIZE;
	m_StepSize = m_Steps[m_StepSizeIndex];
	m_Position = CLongLat( 0.0, 0.0 );
}

CPlayer::~CPlayer()
{
}

double CPlayer::GetStepSize()
{
	return m_StepSize;
}

void CPlayer::SetPosition(double x, double y)
{
	m_Position = CLongLat(x, y);
}

void CPlayer::GetPosition(double& x, double& y)
{
	x = m_Position.longitude();
	y = m_Position.latitude();
}

CLongLat CPlayer::GetPosition()
{
	return m_Position;
}

void CPlayer::Move(int x, int y)
{
	double earthCircumference = 40030.17359;
	double NewX = m_Position.longitude() + x * m_StepSize * 360 / ( earthCircumference* cos(m_Position.latitude()  * M_PI / 180 ) );
	double NewY = m_Position.latitude() + y * m_StepSize * 360 / earthCircumference;

	//Prevent the user going to near the poles which could cause a divide by zero error when moving at the pole 
	if (NewY < MAX_LATITUDE&& NewY > MIN_LATITUDE)
		m_Position.SetLatitude( NewY );
	while (NewX < -180)
		NewX += 360;
	while (NewX > 180)
		NewX -= 360;
m_Position.SetLongitude (NewX );
}

double CPlayer::ChangeStepSize(int change)
{
	if (change < 0 && m_StepSizeIndex > 0)
		m_StepSizeIndex--;
	else if (change > 0 && m_StepSizeIndex < m_Steps.size() - 1)
		m_StepSizeIndex++;

	m_StepSize = m_Steps[m_StepSizeIndex];

	return m_StepSize;
}


double CPlayer::ChangeStepSize_old(int change)
{
	double step = DEFAULT_STEP_SIZE;

	if (m_StepSize < DEFAULT_STEP_SIZE && change >0) //If still in small steps although getting bigger 
		step = DEFAULT_STEP_SIZE / 10;
	else if (m_StepSize <= DEFAULT_STEP_SIZE && change < 0) //About to move into small step sizes 
		step = DEFAULT_STEP_SIZE / 10;

	//Apply step with conditions
	if (m_StepSize <= DEFAULT_STEP_SIZE/ 10 && change < 0)
		m_StepSize = 1;
	else if (m_StepSize == 1 && change > 0)
		m_StepSize = DEFAULT_STEP_SIZE / 10;
	else
		m_StepSize += step * change;

	//Not allowed over max value
	if (m_StepSize > MAX_STEP_SIZE)
		m_StepSize = MAX_STEP_SIZE;

	return m_StepSize;
}

void CPlayer::MoveAbsolute(double x, double y)
{
	m_Position.SetLongitude(x);
	m_Position.SetLongitude(y);
}