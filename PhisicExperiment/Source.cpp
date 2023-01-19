#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <math.h>

int wWidth = 800;
int wHeight = 600;
int const delay = 17;


SDL_Window* win = nullptr;
SDL_Renderer* ren = nullptr;

class Point
{
public:
	Point(double dx, double dy)
	{
		x = dx;
		y = dy;
	};

	double x{ 0 };
	double y{ 0 };
};

class Square
{
public:

	Point A{ 0,0 };
	Point B{ 0,0 };
	Point V{ 0,0 };
	SDL_Color color;

	double speed = 0;

	Square(double& aX, double& aY, double& bX, double& bY, double& vX, double& vY, SDL_Color& dColor, double dSpeed = 1)
	{

		A = { aX, aY };
		B = { bX, bY };
		V = { vX, vY };
		color = dColor;
		speed = dSpeed;

		//swap points for function SDL_RenderFillRect()
		if (B.y > A.y)
			std::swap(B.y, A.y);
		if (B.x < A.x)
			std::swap(B.x, A.x);

	};

	void Show(int x0, int y0)
	{
		double w = abs(B.x - A.x);
		double h = abs(B.y - A.y);
		SDL_Rect rect{ x0 + A.x, y0 + B.y, w, h };

		SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
		SDL_RenderDrawRect(ren, &window);

		//	SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(ren, &rect);
	}

	void Normalize()
	{
		//TEST
		//std::cout << "BEFORE: " << v.x << " " << v.y << "\n";

		double t1 = V.x;
		double t2 = V.y;

		//sqrtl get only positive value
		double l = sqrtl(t1 * t1 + t2 * t2);
		if (l != 0)
		{
			V.x = t1 / l;
			V.y = t2 / l;
		}
		else
		{
			V.x = 0;
			V.y = 0;
		}

		//std::cout << "AFTER: " << v.x << " " << v.y << "\n";
	}

	void Move(bool isGravity)
	{
		if (SDL_abs(A.x) >= window.w / 2 || SDL_abs(B.x) >= window.w / 2)
			V.x *= -1;

		if (SDL_abs(A.y) >= window.h / 2 || SDL_abs(B.y) >= window.h / 2)
			V.y *= -1;

		Normalize();
		double gravity = 1;

		if (isGravity)
		{
			if (window.h / 2 <= A.y)
				V.y *= 0;
			else if (V.y != 0)
			{
				Point g{ 0,-0.1 };

				V.y += g.y * gravity;


			}
		}

		A.x += V.x * speed;
		B.x += V.x * speed;

		A.y -= V.y * speed;
		B.y -= V.y * speed;



		std::cout << (int)A.x << " " << (int)A.y << " " << (int)B.x << " " << (int)B.y << "\n";
	}
	bool ponce = false;
private:

	SDL_Rect window{ 100, 100, wWidth - 200, wHeight - 200 };

};

void Collision(Square& sq1, Square& sq2)
{

	Point c1{ sq1.B.x - abs(sq1.B.x - sq1.A.x) / 2, sq1.B.y - abs(sq1.B.y - sq1.A.y) / 2 };
	Point c2{ sq2.B.x - abs(sq2.B.x - sq2.A.x) / 2, sq2.B.y - SDL_abs(sq2.B.y - sq2.A.y) / 2 };

	//TEST
	//std::cout << sq1.A.x << " " << sq1.A.y << " " << sq1.B.x << " " << sq1.B.y << " ";
	//std::cout << ":" << c1.x << ": :" << c1.y << ":\n";

	double distanceX = abs(c1.x - c2.x);
	double distanceY = abs(c1.y - c2.y);

	double w1 = abs(sq2.B.x - sq2.A.x);
	double w2 = abs(sq1.B.x - sq1.A.x);

	double h1 = abs(sq2.B.y - sq2.A.y);
	double h2 = abs(sq1.B.y - sq1.A.y);

	if (distanceY <= (h1 + h2) / 2 && distanceX <= (w1 + w2) / 2 && sq1.ponce == false && sq2.ponce == false)
	{
		double& X1 = sq1.V.x;
		double& X2 = sq2.V.x;

		double vectorX = X1 * sq1.speed + X2 * sq2.speed;

		double& Y1 = sq1.V.y;
		double& Y2 = sq2.V.y;

		double vectorY = Y1 * sq1.speed + Y2 * sq2.speed;

		//TEST
		//std::cout << "!!!!!!" << vectorX << "!!!!!!" << "!!!!!!" << X1 << "!!!!!!" << "!!!!!!" << X2 << "!!!!!!" << "\n";
		//std::cout << "!!!!!!" << vectory << "!!!!!!" << "!!!!!!" << Y1 << "!!!!!!" << "!!!!!!" << Y2 << "!!!!!!" << "\n";


		X1 = vectorX;
		Y1 = vectorY;
		X2 = vectorX;
		Y2 = vectorY;


		if (sq1.speed > sq2.speed)
		{
			X1 *= -1;
			Y1 *= -1;
		}
		else if (sq1.speed < sq2.speed)
		{
			X2 *= -1;
			Y2 *= -1;
		}

		std::swap(sq1.speed, sq2.speed);

		sq1.ponce = true;
		sq2.ponce = true;
	}

	if (distanceX > (w1 + w2) / 2 || distanceY > (h1 + h2))
	{
		sq1.ponce = false;
		sq2.ponce = false;
	}

}

void showAxes(int x0, int y0)
{
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderDrawLine(ren, x0, y0 - 400, x0, y0 + 400);
	SDL_RenderDrawLine(ren, x0 + 1000, y0, x0 - 1000, y0);
}

double Speed(double pixels)
{
	double FPS = (1000.0 / delay);
	double speed = pixels / FPS;

	return speed;
}

void AddObject()
{

}


int main(int argc, char* arcv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	win = SDL_CreateWindow("Phisic Experiment", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

	ren = SDL_CreateRenderer(win, NULL, SDL_RENDERER_ACCELERATED);



	Point v(100, 51);
	Point A(-100, -20);
	Point B(-60, 20);

	SDL_Color s1{ 255,0,0,255 };
	double speed(0);


	speed = Speed(500);

	Square square1(A.x, A.y, B.x, B.y, v.x, v.y, s1, speed);

	v = { -41, 26 };
	A = { -20, -20 };
	B = { 20, 20 };

	SDL_Color s2{ 255,0,255,255 };

	speed = Speed(400);
	Square square2(A.x, A.y, B.x, B.y, v.x, v.y, s2, speed);

	int x0 = wWidth / 2;
	int y0 = wHeight / 2;

	int timeout = 0;
	double squareTime = SDL_GetTicks64();
	bool isPause = true;
	bool isProcess = true;
	while (isProcess)
	{

		SDL_Event ev;
		while (SDL_PollEvent(&ev))
			switch (ev.type)
			{
			case SDL_KEYDOWN:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE: isProcess = false;
					break;
				case SDL_SCANCODE_SPACE: isPause ? isPause = false : isPause = true;
					break;
				}
				break;
			case SDL_QUIT:isProcess = false;
			}

		if (!isPause)
		{
			square1.Move(1);
			square2.Move(1);
		}

		Collision(square1, square2);

		//	showAxes(x0, y0);


		timeout = SDL_GetTicks64() + delay;
		while (SDL_GetTicks64() < timeout)
		{
			square1.Show(x0, y0);
			square2.Show(x0, y0);
		}

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderPresent(ren);
		SDL_RenderClear(ren);

	}

	SDL_Quit();
	return 0;
}