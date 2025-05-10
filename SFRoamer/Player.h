//Player.h
#pragma once

#include "LongLat.h"
#include<vector>

class CPlayer
{
//Constructors 
public:
	CPlayer();
	~CPlayer();

//Methods
public:
	double GetStepSize();
	void SetPosition(double x, double y);
	void GetPosition(double& x, double& y);
	CLongLat  GetPosition();
	void Move(int x, int y);
	double ChangeStepSize(int change);
	double ChangeStepSize_old(int change);
	void MoveAbsolute(double x, double y);


//Attributes
private:
	CLongLat m_Position;
	double m_StepSize; //In kilometres 
	std::vector<double> m_Steps;
	int m_StepSizeIndex;
};
