//
// * ENGINE-X
// * SAMPLE GAME
//
// + Game.cpp
// implementation of MyGame, an implementation of exGameInterface
//

#include "Game/Private/Game.h"
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <string>
#include "Engine/Public/EngineInterface.h"
#include "Engine/Public/SDL.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------

const char* gWindowName = "Davids Pong";

//-----------------------------------------------------------------
//-----------------------------------------------------------------

MyGame::MyGame()
	: mEngine(nullptr)
	, mFontID(-1)
	, mUp(false)
	, mDown(false)
	, mP2Up(false)
	, mP2Down(false)
	, score1(0)
	,score2(0)
{
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

MyGame::~MyGame()
{
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::Initialize( exEngineInterface* pEngine )
{
	mEngine = pEngine;
	//Initialize the Ball and the two paddles on a certain position
	ball = new Ball();
	player1 = new Paddle(0.0f,100.0f,20.0f,200.0f);
	player2 = new Paddle(779.0f,100.0f,799.0f,200.0f);
	mFontID = mEngine->LoadFont( "Old School Adventures.ttf", 80);

	mTextPosition.x = 250.0f;
	mTextPosition.y = 10.0f;
	mTextPositionP2.x = 450.0f;
	mTextPositionP2.y = 10.0f;

	//set inicial scores
	score1 = 0;
	score2 = 0;

}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

const char* MyGame::GetWindowName() const
{
	return gWindowName;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::GetClearColor( exColor& color ) const
{
	color.mColor[0] = 0;
	color.mColor[1] = 0;
	color.mColor[2] = 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::OnEvent( SDL_Event* pEvent )
{
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::OnEventsConsumed()
{
	int nKeys = 0;
	const Uint8 *pState = SDL_GetKeyboardState( &nKeys );

	mUp = pState[SDL_SCANCODE_W];
	mDown = pState[SDL_SCANCODE_S];
	mP2Up = pState[SDL_SCANCODE_UP];
	mP2Down = pState[SDL_SCANCODE_DOWN];
	
}

//-----------------------------------------------------------------

//-----------------------------------------------------------------

void MyGame::Run( float fDeltaT )
{
	exVector2 lineP, lineP1;
	exColor c;
	float r;

	//middle line
	lineP.x = 400.0f;
	lineP.y = 0.0f;
	lineP1.x = 401.0f;
	lineP1.y = 600.0f;
	c.mColor[0] = 255;
	c.mColor[1] = 255;
	c.mColor[2] = 255;
	c.mColor[3] = 255;
	mEngine->DrawBox(lineP, lineP1, c, 0);
	
	//ball movement
	
	r = 10.0f;
	//left player collides with ball
	if ((ball->getpBallX() > player1->getpFirstPaddleX() && ball->getpBallX() < player1->getpSecondPaddleX() + 9.0f) && (ball->getpBallY() > player1->getpFirstPaddleY() && ball->getpBallY() < player1->getpFirstPaddleY()+40.0f)) {
		ball->changeDirection(UPRIGHT);
	}
	if ((ball->getpBallX() > player1->getpFirstPaddleX() && ball->getpBallX() < player1->getpSecondPaddleX() + 9.0f) && (ball->getpBallY() > player1->getpFirstPaddleY() + 40.0f && ball->getpBallY() < player1->getpSecondPaddleY())) {
		ball->changeDirection(DOWNRIGHT);
	}
	if ((ball->getpBallX() > player1->getpFirstPaddleX() && ball->getpBallX() < player1->getpSecondPaddleX() + 9.0f) && (ball->getpBallY() > player1->getpFirstPaddleY() + 40.0f && ball->getpBallY() < player1->getpSecondPaddleY()-40.0f)) {
		ball->changeDirection(RIGHT);
	}
	//right player collides with ball
	if ((ball->getpBallX() > player2->getpFirstPaddleX() && ball->getpBallX() < player2->getpSecondPaddleX() + 9.0f) && (ball->getpBallY() > player2->getpFirstPaddleY() && ball->getpBallY() < player2->getpFirstPaddleY() + 40.0f)) {
		ball->changeDirection(UPLEFT);
	}
	if ((ball->getpBallX() > player2->getpFirstPaddleX() && ball->getpBallX() < player2->getpSecondPaddleX() + 9.0f) && (ball->getpBallY() > player2->getpFirstPaddleY() + 40.0f && ball->getpBallY() < player2->getpSecondPaddleY())) {
		ball->changeDirection(DOWNLEFT);
	}
	if ((ball->getpBallX() > player2->getpFirstPaddleX() && ball->getpBallX() < player2->getpSecondPaddleX() + 9.0f) && (ball->getpBallY() > player2->getpFirstPaddleY() + 40.0f && ball->getpBallY() < player2->getpSecondPaddleY() - 40.0f)) {
		ball->changeDirection(LEFT);
	}

	//Logic for the walls to collude with
	if (ball->getpBallY() > 590.0f) {
		ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
	}
	if (ball->getpBallY() < 10.0f) {
		ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
	}

	//if the ball collides on left or right wall
	if (ball->getpBallX() > 790.0f) {
		score1++;
		ball->Reset();
	}
	if (ball->getpBallX() < 10.0f) {
		score2++;
		ball->Reset();
	}
	ball->Move(fDeltaT);
	mEngine->DrawCircle( ball->getBall(), r, c, 1 );
	
	//paddle p1
	mEngine->DrawBox( player1->getPaddleP1(), player1->getPaddleP2(), c, 1 );
	//paddle p2
	mEngine->DrawBox(player2->getPaddleP1(), player2->getPaddleP2(), c, 1);
	//score player1
	std::string score1text = "";
	score1text = std::to_string(score1);
	mEngine->DrawText( mFontID, mTextPosition,score1text.c_str(), c, 0 );
	//score player2
	std::string score2text = "";
	score2text = std::to_string(score2);
	mEngine->DrawText(mFontID, mTextPositionP2, score2text.c_str(), c, 0);

	//paddle movement from the two players
	float speed = 300.0f;
	int extraspeed = 2;
	if (mUp && player1->getpFirstPaddleY()>0)
	{
		player1->setpFirstPaddleY(player1->getpFirstPaddleY() - speed * fDeltaT * extraspeed);
		player1->setpSecondPaddleY(player1->getpSecondPaddleY() - speed * fDeltaT * extraspeed);
	}
	else if (mDown && player1->getpSecondPaddleY() <kViewportHeight)
	{
		player1->setpFirstPaddleY(player1->getpFirstPaddleY() + speed * fDeltaT * extraspeed);
		player1->setpSecondPaddleY(player1->getpSecondPaddleY() + speed * fDeltaT * extraspeed);
	}
	if (mP2Up && player2->getpFirstPaddleY() > 0)
	{
		player2->setpFirstPaddleY(player2->getpFirstPaddleY() - speed * fDeltaT * extraspeed);
		player2->setpSecondPaddleY(player2->getpSecondPaddleY() - speed * fDeltaT * extraspeed);
	}
	else if (mP2Down  && player2->getpSecondPaddleY() < kViewportHeight)
	{
		player2->setpFirstPaddleY(player2->getpFirstPaddleY() + speed * fDeltaT * extraspeed);
		player2->setpSecondPaddleY(player2->getpSecondPaddleY() + speed * fDeltaT * extraspeed);
	}

	//if the ball stop change to other position
	if (ball->getDirection() == STOP) {
		ball->randomDirection();
	}
}

