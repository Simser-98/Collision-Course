#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include "config.h"
#include <string>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

// this is used in the Draw_big_star for when changing the position for the small stars
struct SmallDot 
{
	Vector2 position;
	Vector2 velocity;
};


bool Start_menu(Font sysFont, bool boot)
{
	Vector2 mousePos = GetMousePosition();
	Vector2 titlePos{ 1280 / 5, 720 / 3 }; // position of game title
	Vector2 startPos{ 1250 / 3, 720 / 2 }; // position of start game button
	Vector2 startSize{ 150, 25 }; // size of start button
	Rectangle startBtn{ startPos.x, startPos.y, startSize.x, startSize.y };
	
	
	if (!boot)
	{
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTextEx(sysFont, "Collision Course", titlePos, 75, 1, WHITE); // draws title
		DrawTextEx(sysFont, "Start Game", startPos, 25, 1, WHITE); // draw start button
		//DrawRectangleV(startPos, startSize, HURTBOX);
		EndDrawing();
		
	}
	if (CheckCollisionPointRec(mousePos, startBtn) and IsMouseButtonPressed(0)) // checks if left clicked start button
	{
		return true; // if left clicked

	}
	return false;
	
}



bool Game_over(Font sysFont, bool boot, float Timer)
{
	Vector2 mousePos = GetMousePosition();
	Vector2 titlePos{ 1280 / 4, 720 / 3 }; // position of game over text
	Vector2 menuPos{ 1280 / 3, 720 / 1.5 }; // position of menu button
	Vector2 menuSize{ 280, 25 };
	Vector2 scoreTextPos{ 1280 / 3, 720 / 2 }; // position of text to tell your score
	Vector2 scorePos{ 1280 / 3 + 290, 720 / 2 }; // position of the score
	Rectangle menuBtn{ menuPos.x, menuPos.y, menuSize.x, menuSize.y };
	char gameTime[50];
	sprintf_s(gameTime, "%.3f", Timer);

	BeginDrawing();
	ClearBackground(BLACK);
	//DrawRectangleV(menuPos, menuSize, HURTBOX);
	DrawTextEx(sysFont, "Game Over", titlePos, 100, 1, WHITE);
	DrawTextEx(sysFont, "back to Main Menu", menuPos, 30, 1, WHITE);
	DrawTextEx(sysFont, "you survived for: ", scoreTextPos, 30, 1, WHITE);
	DrawTextEx(sysFont, gameTime, scorePos, 50, 1, WHITE);
	EndDrawing();

	
	
	if (CheckCollisionPointRec(mousePos, menuBtn) and IsMouseButtonPressed(0)) // checks if left clicked main menu button
	{
		boot = true;
		return true;

	}
	return false;
	
	
}



bool Damage_collision_detection(vector<Vector2>& bPos, Vector2 objPos, vector<Vector2> bStarPos, vector<SmallDot> sbPos )
{
	Vector2 hurtboxBaseSize{ 64 / 4, 64 }; // size of hurtbox for the back of the ship sprite
	Vector2 hurtboxBaseOrigin{ objPos.x - 32, objPos.y - 32 }; // setting the center of the hurtbox
	Vector2 hurtboxShaftSize{ 50, 64 / 3.2 }; // size of hurtbox of front of ship sprite
	Vector2 hurtboxShaftOrigin{ objPos.x - 16, objPos.y - 10 }; // setting center of the hurtbox 
	Rectangle baseBox{ hurtboxBaseOrigin.x, hurtboxBaseOrigin.y, hurtboxBaseSize.x, hurtboxBaseSize.y }; // creates a retangle with the position and dimention for the hurtbox of base of ship
	Rectangle shaftBox{ hurtboxShaftOrigin.x, hurtboxShaftOrigin.y, hurtboxShaftSize.x, hurtboxShaftSize.y }; // creates a retangle with the position and dimention for the hurtbox of shaft of ship
	//DrawRectangleV(hurtboxBaseOrigin, hurtboxBaseSize, HURTBOX);
	//DrawRectangleV(hurtboxShaftOrigin, hurtboxShaftSize, HURTBOX);
	for (int i = 0; i < bPos.size(); i++)
	{
		if (CheckCollisionCircleRec(bPos[i], 2, baseBox) or CheckCollisionCircleRec(bPos[i], 2, shaftBox)) // check if position of a star collide with the position of the retangles for the hurtboxes
		{
			
			return true;
		}
		for (int i = 0; i < bStarPos.size(); i++)
		{
			if (CheckCollisionCircleRec(bStarPos[i], 2, baseBox) or CheckCollisionCircleRec(bStarPos[i], 2, shaftBox)) // checks if the hurtboxes collide with the big stars
			{
				
				return true;
			}
		}
		for (int i = 0; i < sbPos.size(); i++)
		{
			if (CheckCollisionCircleRec(sbPos[i].position, 2, baseBox) or CheckCollisionCircleRec(sbPos[i].position, 2, shaftBox)) // checks if the stars made from the big stars collide with the ship hurtbox
			{
				
				return true;
			}
		}
	}
	return 0;
}



void Draw_ship(Vector2 objPos, Texture2D objTxr, Font sysFont , float Timer, bool boot, float iFrame, vector<Vector2>& Pos,  vector<Vector2> bStarPos, vector<SmallDot> sbPos)
{
	
	Rectangle source{0,0, (float)objTxr.width, (float)objTxr.height};
	Rectangle dest{objPos.x, objPos.y, 64, 64};
	Vector2 origin{64/2, 64/2};
	Vector2 fpsPos{ 10, 10 };
	Vector2 timerPos{ 100, 10 };
	string gameFPS = to_string(GetFPS()); //converts integer into a string
	char gameTime[50]; // used for drawing the fps
	float flashInterval = 0.1f; // Interval for toggling visibility
	static bool spriteVisible = true; // Visibility state
	static bool isFlashing = false;   // Is the sprite currently flashing?
	bool isHit = Damage_collision_detection(Pos, objPos, bStarPos, sbPos);

	// Handle collision and flashing
	if (isHit && !isFlashing) 
	{
		isFlashing = true;   // Start flashing effect
		iFrame = 2.0f;       // Set flashing duration
	}

	if (isFlashing) 
	{
		iFrame -= deltaTime; // Decrease flashing timer

		// Toggle visibility based on flash interval
		if (((int)(iFrame / flashInterval) % 2) == 0) 
		{
			spriteVisible = true;
		}
		else 
		{
			spriteVisible = false;
		}

		// Stop flashing when timer ends
		if (iFrame <= 0.0f) 
		{
			isFlashing = false;
			spriteVisible = true; // Ensure sprite is visible at the end
		}
	}

	sprintf_s(gameTime, "%.3f", Timer);
	if (!boot)
	{
		BeginDrawing();
		ClearBackground(BLACK);
		if (spriteVisible)
		{
			DrawTexturePro(objTxr, source, dest, origin, 90.f, WHITE); // draws sprite onto screen
		}
		DrawTextEx(sysFont, gameFPS.c_str(), fpsPos, 30, 1, WHITE); // draw fps onto screen
		DrawTextEx(sysFont, gameTime, timerPos, 30, 1, WHITE); // draws timer onto screen
		EndDrawing();
	}
	
	
}



void Lives_display(Texture2D objTxr, int Lives)
{
	
	Rectangle source{ 0,0, (float)objTxr.width, (float)objTxr.height };
	Rectangle l1Dest{ 400, 20, 32, 32 };
	Vector2 Origin{ 32 / 2, 32 / 2 };
	Rectangle l2Dest{ 430, 20, 32, 32 };
	Rectangle l3Dest{ 460, 20, 32, 32 };
	if (Lives >= 3)
	{
		DrawTexturePro(objTxr, source, l1Dest, Origin, 0.f, WHITE);
	}
	if (Lives >= 2)
	{
		DrawTexturePro(objTxr, source, l2Dest, Origin, 0.f, WHITE);
	}
	if (Lives >= 1)
	{
		DrawTexturePro(objTxr, source, l3Dest, Origin, 0.f, WHITE);
	}
	
}



void Draw_star( vector<Vector2>& Pos , bool boot)
{
	static float StarSpawnTimer = 0.0f;
	if (!boot) 
	{
		StarSpawnTimer += deltaTime;
		if (StarSpawnTimer >= 1.0f) // rate of increase of stars on screen
		{ 
			StarSpawnTimer = 0.0f;
			Pos.push_back(Vector2{ (float)GetRandomValue(1280, 2000), (float)GetRandomValue(1, 720)}); // add a star to the start of the array
		}
	}

	// Move stars and draw them
	for (int i = 0; i < Pos.size(); i++) 
	{
		if (Pos[i].x < 0) 
		{
			// Recycle stars that leave the screen
			Pos[i] = Vector2{ (float)GetRandomValue(1280, 2000), (float)GetRandomValue(1, 720)};
		}
		Pos[i].x -= 400 * deltaTime;

		if (!boot) 
		{
			DrawCircleV(Pos[i], 2, WHITE);
		}
	}
} 



void Draw_big_star(vector<Vector2>& bStarPos,vector<SmallDot>& sPos ,bool boot)
{
	static float bigStarSpawnTimer = 0.0f;
	static int numSmallDots = 15;
	float bigStarSpeed = 400.0f; // Speed of big stars
	float smallDotSpeed = 200.0f; // Speed of small dots

	if (!boot) 
	{
		bigStarSpawnTimer += deltaTime;
		
	}
	if (bigStarSpawnTimer >= 4.8f) // rate of big stars added to the screen
	{
		bStarPos.push_back(Vector2{ (float)GetRandomValue(1280, 2000), (float)GetRandomValue(1, 720) }); // adds a big star to the start of the array
		bigStarSpawnTimer = 0.0f;
	}
	// === Handle Big Stars ===
	for (size_t i = 0; i < bStarPos.size(); i++) 
	{
		if (bStarPos[i].x < 10) 
		{
			
			// Spawn small dots at the position of the big star
			for (int j = 0; j < numSmallDots; j++)
			{
				float angle = 2.0f * PI * j / numSmallDots; // Spread evenly in a circle
				Vector2 velocity = { cosf(angle) * smallDotSpeed, sinf(angle) * smallDotSpeed };

				// Add the dot at the big star's position
				sPos.push_back({ bStarPos[i], velocity });
			}

			// Recycle the big star
			bStarPos[i] = Vector2{ (float)GetRandomValue(1280, 2000), (float)GetRandomValue(1, 720) };
		}

		// Move the big star
		bStarPos[i].x -= bigStarSpeed * deltaTime;

		// Draw the big star
		if (!boot) 
		{
			DrawCircleV(bStarPos[i], 10, WHITE);
		}
	}

	// === Handle Small Dots ===
	for (size_t i = 0; i < sPos.size(); /* no increment */) 
	{
		// Update position based on velocity
		sPos[i].position.x += sPos[i].velocity.x *deltaTime;
		sPos[i].position.y += sPos[i].velocity.y * deltaTime;

		// Check if the dot is off-screen
		if (sPos[i].position.x < 0 or sPos[i].position.x > 1280 or sPos[i].position.y < 0 or sPos[i].position.y > 720) 
		{
			// Remove the dot if it goes off-screen
			sPos.erase(sPos.begin() + i);
		}
		else 
		{
			// Draw the dot and move to the next
			if (!boot) 
			{
				DrawCircleV(sPos[i].position, 2, WHITE);
			}
			i++;
		}
	}

    
}



void Draw_heal(Texture2D objTxr, int Lives, Vector2& lifePos, bool boot )
{
	Rectangle source{ 0,0, (float)objTxr.width, (float)objTxr.height };
	Vector2 Origin{ 27 / 2, 27 / 2 };
	Rectangle healthDest{ lifePos.x, lifePos.y, 27, 27 };
	static float shipRotation = 0.0f;
	
	if (!boot)
	{

		if (Lives < 3)
		{
			shipRotation += deltaTime * 100;
			if (shipRotation >= 360.0f)
			{
				shipRotation = 0.0f;
			}

			if (lifePos.x < 0)
			{
				// Recycle health item that leave the screen
				lifePos = Vector2{ (float)GetRandomValue(2000 ,4000), (float)GetRandomValue(1, 720) };
			}
			lifePos.x -= 400 * deltaTime;
			DrawTexturePro(objTxr, source, healthDest, Origin, shipRotation, WHITE);
		}
		if (Lives > 2)
		{
			lifePos = Vector2{ (float)GetRandomValue(2000, 4000), (float)GetRandomValue(1, 720) }; // obtains item reset the item position
		}
	}
	
}



bool Heal_collision(Vector2 lifePos,Vector2 objPos ,int Lives)
{
	Vector2 hurtboxBaseSize{ 64 / 4, 64 };
	Vector2 hurtboxBaseOrigin{ objPos.x - 32, objPos.y - 32 };
	Vector2 hurtboxShaftSize{ 50, 64 / 3.2 };
	Vector2 hurtboxShaftOrigin{ objPos.x - 16, objPos.y - 10 };
	Rectangle baseBox{ hurtboxBaseOrigin.x, hurtboxBaseOrigin.y, hurtboxBaseSize.x, hurtboxBaseSize.y };
	Rectangle shaftBox{ hurtboxShaftOrigin.x, hurtboxShaftOrigin.y, hurtboxShaftSize.x, hurtboxShaftSize.y };
	Rectangle healthBox{ lifePos.x, lifePos.y, 27, 27 };
	if (CheckCollisionRecs(baseBox, healthBox) or CheckCollisionRecs(shaftBox, healthBox))
	{
		return true;
	}
	return false;
}






int main()
{
	
	InitWindow(1280, 720, "Collision Course");
	float masterVol = 0.5f;
	InitAudioDevice();
	SetMasterVolume(masterVol);
	SetTargetFPS(99999999);
	bool gameStart = false; // boolean value for function
	bool trueStart = false; // boolean value for keeping game started
	bool dead = false; // boolean value for function
	bool gameEnd = false; // boolean value for keeping game ended
	
	int shipLives = 3;
	float iTime = 0.0f; // invincibility timer
	float gameTimer = 0.0f;
	float speedMult = 1.0f;

	Vector2 shipPos = { 400, 400 };
	Vector2 healthPos = { 0, 0 };
	vector<Vector2> StarPos;
	vector<Vector2> bigStarPos;
	vector<SmallDot> smallStarPos;

	
	Image userShipImg = LoadImage("assets/ship.png");
	Texture2D userShipTxr = LoadTextureFromImage(userShipImg);
	Font gameFont = LoadFont("assets/font.ttf");
	Sound gameTheme = LoadSound("assets/theme.mp3");
	
	
	

	
//=============================================================================
	while(WindowShouldClose() == false)
	{
		deltaTime = GetFrameTime();
		if (IsKeyPressed(KEY_M) and masterVol != 0.0f)
		{
			masterVol = 0.0f;
			SetMasterVolume(masterVol);
		}
		else if (IsKeyPressed(KEY_M))
		{
			masterVol = 0.4f;
			SetMasterVolume(masterVol);
		}
		
		
		
		
		if (!trueStart and !gameEnd)
		{
			gameStart = Start_menu(gameFont, trueStart);
			if (gameStart) // sets all values back to initial values when gaem restarts
			{
				PlaySound(gameTheme);
				trueStart = true;
				shipPos = { 400, 400 };
				shipLives = 3;
				gameTimer = 0.0f;
				iTime = 0.0f;
				StarPos.clear();
				smallStarPos.clear();
				bigStarPos.clear();
				
			}
		}
		else if (gameEnd)
		{
				dead = Game_over(gameFont, gameEnd, gameTimer);
				
				StopSound(gameTheme);
				
				if (dead)
				{
					gameEnd = false;
					trueStart = false;
					gameStart = false;
					

					
				}
		}
		else
		{
			
			Vector2 shipMove = { 0,0 };
			gameTimer += deltaTime;
// ============================================================================
			
			if ((IsKeyDown(KEY_D) or IsKeyDown(KEY_RIGHT)) and (shipPos.x < 1280))
			{
				shipMove.x += 1;
			}
			
			if ((IsKeyDown(KEY_A) or IsKeyDown(KEY_LEFT)) and (shipPos.x > 0))
			{
				shipMove.x -= 1;
			}
			
			if ((IsKeyDown(KEY_W) or IsKeyDown(KEY_UP)) and (shipPos.y > 0))
			{
				shipMove.y -= 1;
			}
			
			if ((IsKeyDown(KEY_S) or IsKeyDown(KEY_DOWN)) and (shipPos.y < 720))
			{
				shipMove.y += 1;
			}
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				speedMult = 0.65f;
			}
			else
			{
				speedMult = 1.0f;
			}
			shipPos = Vector2Add(shipPos, Vector2Normalize(shipMove) * deltaTime * speed * speedMult); // so movement speed is consistent for diagonals
			
			// ============================================================================	

			Draw_ship(shipPos, userShipTxr, gameFont, gameTimer, gameEnd, iTime ,  StarPos, bigStarPos, smallStarPos);
			Lives_display(userShipTxr, shipLives);
			Draw_star(StarPos, gameEnd);
			Draw_big_star(bigStarPos, smallStarPos, gameEnd);
			Draw_heal(userShipTxr, shipLives, healthPos, gameEnd);
			
			bool isHealed = false;
			bool isHit = false;
			isHit = Damage_collision_detection(StarPos, shipPos, bigStarPos, smallStarPos);
			isHealed = Heal_collision(healthPos, shipPos, shipLives);
			
			// changing health with colision and invincibility time
			if (isHit == 1 and iTime == 0.f)
			{
				shipLives -= 1;
				iTime = 2.f;
			}

			if (iTime <= 2.f and iTime > 0.f)
			{
				iTime -= deltaTime;
				
			}
			else if (iTime < 0.f)
			{
				iTime = 0.f;
			}
			if (shipLives <= 0)
			{
				gameEnd = true;
			}
			if (isHealed == 1 and shipLives == 2)
			{
				shipLives = 3;

			}
			else if (isHealed == 1 and shipLives == 1)
			{
				shipLives = 2;
			}
			
			
		}
		
	}
	

	// unloading assests
	UnloadFont(gameFont);
	UnloadSound(gameTheme);
	UnloadImage(userShipImg);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}

