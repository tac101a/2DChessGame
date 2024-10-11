#include "MoveMent.h"

MoveMent::MoveMent()
{
	/*this->c1 = nullptr;
	this->c2 = nullptr;
	this->tempKill = nullptr;
	this->tempMove = nullptr;*/
	this->c1 = ChessCell();
	this->c2 = ChessCell();
	this->tempKill = ChessCell();
	this->tempMove = ChessCell();
}

MoveMent::~MoveMent()
{
	/*delete this->c1;
	delete this->c2;
	delete this->tempKill;
	delete this->tempMove;*/
}

void MoveMent::setC1(ChessCell cp) {
	this->c1 = cp;
}
void MoveMent::setC2(ChessCell cp) {
	this->c2 = cp;
}
void MoveMent::setTempKill(ChessCell cp) {
	this->tempKill = cp;
}
void MoveMent::setTempMove(ChessCell  cp) {
	this->tempMove = cp;
}


ChessCell MoveMent::getC2()
{
	return this->c2;
}
ChessCell MoveMent::getC1()
{
	return this->c1;
}
ChessCell MoveMent::getTempKill()
{
	return this->tempKill;
}
ChessCell MoveMent::getTempMove()
{
	return this->tempMove;
}

void MoveMent::setC1_(int c1_)
{
	this->c1_ = c1_;
}

void MoveMent::setC2_(int c2_)
{
	this->c2_ = c2_;
}

void MoveMent::setTempKill_(int cK_)
{
	this->cK_ = cK_;
}

void MoveMent::setTempMove_(int cM_)
{
	this->cM_ = cM_;
}

int MoveMent::getC2_()
{
	return c2_;
}

int MoveMent::getC1_()
{
	return c1_;
}

int MoveMent::getTempKill_()
{
	return cK_;
}

int MoveMent::getTempMove_()
{
	return cM_;
}



