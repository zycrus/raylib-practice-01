#include "raylib.h"

struct Ball
{
	float radius = 0.0f;
	float prevVelX = 0.0f;
	Vector2 pos = { 0, 0 };
	Vector2 vel = { 0, 0 };

	Color color = WHITE;

	void Update()
	{
		pos.x += vel.x * GetFrameTime();
		pos.y += vel.y * GetFrameTime();

		if (pos.y < 0)
		{
			pos.y = 0;
			vel.y *= -1.1f;
		}
		if (pos.y >= (float) GetScreenHeight())
		{
			pos.y = (float) GetScreenHeight();
			vel.y *= -1.1f;
		}
	}

	void Draw()
	{
		DrawCircleV(pos, radius, color);
	}
};

struct Paddle
{
	Vector2 size = { 10.0f, 125.0f };
	Vector2 pos = { 0.0F, 0.0F };
	float speed = 500.0f;

	int score = 0;
	const char* side = "";

	Rectangle GetRect()
	{
		return { pos.x, pos.y, size.x, size.y };
	}

	void SetAttributes(Vector2 _pos, const char* _side)
	{
		pos = {_pos.x - size.x / 2, _pos.y - size.y / 2 };
		side = _side;
	}

	void Draw()
	{
		DrawRectangleRec(GetRect(), WHITE);
	}

	void DrawScore()
	{
		const char* scoreText = TextFormat("%i", score);
		int textWidth = 0;
		if (side == "Left")
			textWidth = MeasureText(scoreText, 100) + 50;
		if (side == "Right")
			textWidth = -50;

		DrawText(scoreText, GetScreenWidth() / 2 - textWidth, GetScreenHeight() / 2 - 50, 100, LIGHTGRAY);
	}

	void Move(KeyboardKey keyUp, KeyboardKey keyDown)
	{
		if (IsKeyDown(keyUp))
		{
			if (pos.y + size.y / 2 > 0)
				pos.y -= speed * GetFrameTime();
			else
				pos.y = 0.0f - size.y / 2 ;
		}
		if (IsKeyDown(keyDown))
		{
			if (pos.y + size.y / 2 < GetScreenHeight())
				pos.y += speed * GetFrameTime();
			else
				pos.y = (float) GetScreenHeight() - size.y / 2;
		}
	}
};

// Global Variables

const int winWidth = 700;
const int winHeight = 500;

Ball ball;
Paddle leftPaddle;
Paddle rightPaddle;

const char* winnerText = nullptr;
const char* detailsText = nullptr;

void Update()
{
	// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
	ball.Update();

	leftPaddle.Move(KEY_W, KEY_S);
	rightPaddle.Move(KEY_UP, KEY_DOWN);

	if (CheckCollisionCircleRec(ball.pos, ball.radius, leftPaddle.GetRect()))
	{
		if (ball.vel.x < 0)
		{
			ball.vel.x *= -1.1f;
			ball.vel.y = (ball.pos.y - leftPaddle.pos.y) / (leftPaddle.size.y / 2) * ball.vel.x;
		}
	}
	if (CheckCollisionCircleRec(ball.pos, ball.radius, rightPaddle.GetRect()))
	{
		if (ball.vel.x > 0)
		{
			ball.vel.x *= -1.1f;
			ball.vel.y = (ball.pos.y - rightPaddle.pos.y) / (rightPaddle.size.y / 2) * -ball.vel.x;
		}
	}

	if (ball.pos.x < 0)
	{
		rightPaddle.score += 1;
		winnerText = "Right Player Wins!";
		ball.pos = { (float)winWidth / 2, (float)winHeight / 2 };
		ball.vel = { 0.0f, 0.0f };
		ball.color = BLACK;
	}
	if (ball.pos.x > GetScreenWidth())
	{
		leftPaddle.score += 1;
		winnerText = "Left Player Wins!";
		ball.pos = { (float)winWidth / 2, (float)winHeight / 2 };
		ball.vel = { 0.0f, 0.0f };
		ball.color = BLACK;
	}

	if (winnerText && IsKeyPressed(KEY_SPACE))
	{
		ball.color = WHITE;
		ball.vel = { -ball.prevVelX, 0.0f };
		ball.prevVelX = ball.vel.x;
		winnerText = nullptr;

		leftPaddle.SetAttributes({ (float)30, (float)winHeight / 2 }, "Left");
		rightPaddle.SetAttributes({ (float)winWidth - 30, (float)winHeight / 2 }, "Right");
	}
}

void Draw()
{
	// Draw
		//----------------------------------------------------------------------------------
	BeginDrawing();

	ClearBackground(BLACK);


	if (winnerText != nullptr)
	{
		int textWidth = MeasureText(winnerText, 60);
		DrawText(winnerText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 - 45, 60, WHITE);

		detailsText = "Press SPACE to Continue";
		int descTextWidth = MeasureText(detailsText, 30);
		DrawText(detailsText, (GetScreenWidth() - descTextWidth) / 2, GetScreenHeight() / 2 + 45, 30, LIGHTGRAY);
	}
	else
	{
		ball.Draw();
		rightPaddle.Draw();
		leftPaddle.Draw();
		leftPaddle.DrawScore();
		rightPaddle.DrawScore();

		DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), LIGHTGRAY);
	}

	// Display FPS
	DrawFPS(10, 10);

	EndDrawing();
}

void UpdateDrawFrame()
{
	Update();
	Draw();
}

int main()
{

	ball.radius = 5.0f;
	ball.pos = { (float)winWidth / 2, (float)winHeight / 2 };
	ball.vel = { 300.0f, 0.0f };
	ball.prevVelX = ball.vel.x;

	leftPaddle.SetAttributes({ (float)30, (float)winHeight / 2 }, "Left");
	rightPaddle.SetAttributes({ (float)winWidth - 30, (float)winHeight / 2 }, "Right");

	InitWindow(winWidth, winHeight, "Pong");

	// VSYNC -- SetWindowState(FLAG_VSYNC_HINT);
	// Set FPS
	SetTargetFPS(60);

	while (!WindowShouldClose())// Detect window close button or ESC key
	{
		UpdateDrawFrame();
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;


}