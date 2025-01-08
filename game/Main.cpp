#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <string>
#include <iostream>
#include <random>
#include <vector>


// this is used in the Draw_big_star for when changing the position for the small stars
struct SmallDot 
{
	Vector2 position;
	Vector2 velocity;
};


bool Start_menu(const Font& sysFont, bool boot, float winW , float winH )
{
	Vector2 mousePos = GetMousePosition();
	Vector2 titlePos{winW / 5, winH / 3 }; // position of game title
	Vector2 startPos{ winW / 3, winH / 2 }; // position of start game button
	Vector2 startSize{ 150, 25 }; // size of start button
	Rectangle startBtn{ startPos.x, startPos.y, startSize.x, startSize.y };
	
	
	if (!boot)
	{
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTextEx(sysFont, "Collision Course", titlePos, 75, 1, WHITE);
		DrawTextEx(sysFont, "Start Game", startPos, 25, 1, WHITE);
		//DrawRectangleV(startPos, startSize, HURTBOX);
		EndDrawing();
		
	}
	if (CheckCollisionPointRec(mousePos, startBtn) and IsMouseButtonPressed(0)) // checks if left clicked start button
	{
		return true;

	}
	return false;
	
}



bool Game_over( const Font& sysFont, bool boot, float Timer, float winW, float winH) // relative pos needed
{
	Vector2 mousePos = GetMousePosition();
	Vector2 titlePos{ winW / 4, winH / 3 }; // position of game over text
	Vector2 menuPos{ winW / 3, winH / 1.5f }; // position of menu button
	Vector2 menuSize{ 280, 25 };
	Vector2 scoreTextPos{ winW / 3, winH / 2 }; // position of text to tell your score
	Vector2 scorePos{ winW / 3 + 290, winW / 2 }; // position of the score
	Rectangle menuBtn{ menuPos.x, menuPos.y, menuSize.x, menuSize.y };
	char gameTime[50];
	sprintf_s(gameTime, "%.3f", Timer);

	BeginDrawing();
	ClearBackground(BLACK);
	//DrawRectangleV(menuPos, menuSize, DARKGREEN);
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



bool Damage_collision_detection( const std::vector<Vector2>& bPos, const Vector2& objPos, 
	const std::vector<Vector2>& bStarPos, const std::vector<SmallDot>& sbPos )
{
	Vector2 hurtboxBaseSize{ 64 / 4, 64 }; 
	Vector2 hurtboxBaseOrigin{ objPos.x - 32, objPos.y - 32 }; 
	Vector2 hurtboxShaftSize{ 50, 64 / 3.2 }; 
	Vector2 hurtboxShaftOrigin{ objPos.x - 16, objPos.y - 10 };  
	Rectangle baseBox{ hurtboxBaseOrigin.x, hurtboxBaseOrigin.y, hurtboxBaseSize.x, hurtboxBaseSize.y }; // creates a retangle with the position and dimention for the hurtbox of base of ship
	Rectangle shaftBox{ hurtboxShaftOrigin.x, hurtboxShaftOrigin.y, hurtboxShaftSize.x, hurtboxShaftSize.y }; // creates a retangle with the position and dimention for the hurtbox of shaft of ship
	//DrawRectangleV(hurtboxBaseOrigin, hurtboxBaseSize, GREEN);
	//DrawRectangleV(hurtboxShaftOrigin, hurtboxShaftSize, GREEN);
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


// relative pos needed
void Draw_ship( const Vector2& objPos,  const Texture2D& objTxr,  const Font& sysFont , float Timer, bool boot,
	float iFrame, const std::vector<Vector2>& Pos, const std::vector<Vector2>& bStarPos,  const std::vector<SmallDot>& sbPos, float deltaTime) 
{
	
	Rectangle source{0,0, (float)objTxr.width, (float)objTxr.height};
	Rectangle dest{objPos.x, objPos.y, 64, 64};
	Vector2 origin{64/2, 64/2};
	Vector2 fpsPos{ 10, 10 };
	Vector2 timerPos{ 100, 10 };
	char gameTime[50]; // used for drawing the fps
	float flashInterval = 0.1f; // Interval for toggling visibility
	static bool spriteVisible = true; // Visibility state
	static bool isFlashing = false;   // Is the sprite currently flashing?
	bool isHit = Damage_collision_detection(Pos, objPos, bStarPos, sbPos);

	// Handle collision and flashing
	if (isHit and !isFlashing) 
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
		DrawTextEx(sysFont, std::to_string(GetFPS()).c_str(), fpsPos, 30, 1, WHITE); // draw fps onto screen
		DrawTextEx(sysFont, gameTime, timerPos, 30, 1, WHITE); // draws timer onto screen
		EndDrawing();
	}
	
	
}



void Lives_display(const Texture2D& objTxr, int Lives, float winW,  float winH)
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



void Draw_star( std::vector<Vector2>& Pos , bool boot, float deltaTime, float winW, float winH)
{
	static float StarSpawnTimer = 0.0f;
	if (!boot) 
	{
		StarSpawnTimer += deltaTime;
		if (StarSpawnTimer >= 1.0f) // rate of increase of stars on screen
		{ 
			StarSpawnTimer = 0.0f;
			Pos.push_back(Vector2{ (float)GetRandomValue((int)winW, 2000), (float)GetRandomValue(1, (int)winH)}); // add a star to the start of the array
		}
	}

	// Move stars and draw them
	for (int i = 0; i < Pos.size(); i++) 
	{
		if (Pos[i].x < 0) 
		{
			// Recycle stars that leave the screen
			Pos[i] = Vector2{ (float)GetRandomValue((int)winW, 2000), (float)GetRandomValue(1, (int)winH)};
		}
		Pos[i].x -= 400 * deltaTime;

		if (!boot) 
		{
			DrawCircleV(Pos[i], 2, WHITE);
		}
	}
} 



void Draw_big_star(std::vector<Vector2>& bStarPos,std::vector<SmallDot>& sPos ,bool boot, float deltaTime, float winW, float winH)
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
		bStarPos.push_back(Vector2{ (float)GetRandomValue((int)winW, 2000), (float)GetRandomValue(1, (int)winH) }); // adds a big star to the start of the array
		bigStarSpawnTimer = 0.0f;
	}
	// === Handle Big Stars ===
	for (auto i = 0; i < bStarPos.size(); i++) 
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
			bStarPos[i] = Vector2{ (float)GetRandomValue((int)winW, 2000), (float)GetRandomValue(1, (int)winH) };
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
	for (auto i = 0; i < sPos.size(); /* no increment */) 
	{
		// Update position based on velocity
		sPos[i].position.x += sPos[i].velocity.x *deltaTime;
		sPos[i].position.y += sPos[i].velocity.y * deltaTime;

		// Check if the dot is off-screen
		if (sPos[i].position.x < 0 or sPos[i].position.x > winW or sPos[i].position.y < 0 or sPos[i].position.y > winH) 
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



void Draw_heal( const Texture2D& objTxr, int Lives, Vector2& lifePos, bool boot , float deltaTime, float winH)
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
				lifePos = Vector2{ (float)GetRandomValue(2000 ,4000), (float)GetRandomValue(1, (int)winH) };
			}
			lifePos.x -= 400 * deltaTime;
			DrawTexturePro(objTxr, source, healthDest, Origin, shipRotation, WHITE);
		}
		if (Lives > 2)
		{
			lifePos = Vector2{ (float)GetRandomValue(2000, 4000), (float)GetRandomValue(1, (int)winH) }; // obtains item reset the item position
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
	const float WINHEIGHT = 720.0f;
	const float WINWIDTH = 1280.0f;
	InitWindow(static_cast<int> (WINWIDTH), static_cast<int> (WINHEIGHT), "Collision Course");
	float deltaTime = GetFrameTime();
	SetTargetFPS(0);

	float masterVol = 0.15f;
	SetMasterVolume(masterVol);
	InitAudioDevice();


	bool gameStart = false; // boolean value for function
	bool trueStart = false; // boolean value for keeping game started
	bool dead = false; // boolean value for function
	bool gameEnd = false; // boolean value for keeping game ended
	
	
	int shipLives;
	float iTime; // invincibility timer
	float gameTimer;
	float shipSpeed = 315.0f;
	float speedMult;

	Vector2 shipPos;
	Vector2 healthPos = { 0, 0 };
	std::vector<Vector2> StarPos;
	std::vector<Vector2> bigStarPos;
	std::vector<SmallDot> smallStarPos;

	
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
			masterVol = 0.15f;
			SetMasterVolume(masterVol);
		}
		
		
		
		
		if (!trueStart and !gameEnd)
		{
			gameStart = Start_menu(gameFont, trueStart, WINWIDTH, WINHEIGHT);
			if (gameStart) // sets all values back to initial values when game restarts
			{
				PlaySound(gameTheme);
				trueStart = true;
				shipPos = { WINWIDTH / 3, WINHEIGHT / 2 };
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
				dead = Game_over(gameFont, gameEnd, gameTimer, WINWIDTH, WINHEIGHT);
				
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
			shipPos = Vector2Add(shipPos, Vector2Normalize(shipMove) * deltaTime * shipSpeed * speedMult); // so movement speed is consistent for diagonals
			
			// ============================================================================	

			Draw_ship(shipPos, userShipTxr, gameFont, gameTimer, gameEnd, iTime ,  StarPos, bigStarPos, smallStarPos, deltaTime);
			Lives_display(userShipTxr, shipLives, WINWIDTH, WINHEIGHT);
			Draw_star(StarPos, gameEnd, deltaTime, WINWIDTH, WINHEIGHT);
			Draw_big_star(bigStarPos, smallStarPos, gameEnd, deltaTime, WINWIDTH, WINHEIGHT);
			Draw_heal(userShipTxr, shipLives, healthPos, gameEnd, deltaTime, WINHEIGHT);
			
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

