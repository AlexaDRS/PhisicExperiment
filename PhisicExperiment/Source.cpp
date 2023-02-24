#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <math.h>
#include <vector>

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

class Figure
{
public:
	//Points of figure
	 std::vector <Point> points;

	//Vector
	Point V{ 0,0 };

	SDL_Color color;

	double speed = 0;

	Figure(std::vector <Point> sPoints, double vX, double vY, SDL_Color& dColor, double dSpeed = 1)
	{
		for (int i = 0; i < sPoints.size(); i++)
		{	
			points.push_back(sPoints[i]);
		}
		std::cout << points.size();
		V = { vX, vY };
		color = dColor;
		speed = dSpeed;
	};

	void Show(int x0, int y0)
	{

		SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
		SDL_RenderDrawRect(ren, &window);

		Point A{ 0,0 };
		Point B{ 0,0 };

		for (int i = 0; i < points.size() - 1; ++i)
		{
			A = { x0 + points[i].x, y0 - points[i].y };
			B = { x0 + points[i+1].x, y0 - points[i+1].y };
			SDL_RenderDrawLine(ren, A.x, A.y, B.x, B.y);
		}
		A = { x0 + points[0].x, y0 - points[0].y };
		SDL_RenderDrawLine(ren, A.x, A.y, B.x, B.y);
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

	Point CenterMass()
	{
		double mX(0);
		double mY(0);
		//double m(0);
		for (int i = 0; i < points.size(); ++i)
		{
			mX += points[i].x;
			mY += points[i].y;
		}

		mX /= points.size();
		mY /= points.size();

		
		return { mX, mY };
	}

	void Rotate(double angle, double x0, double y0, Point &C)
	{
		double radian = angle * M_PI / 180.0;

		bool isGood = true;;
		for (int i = 0; i < points.size(); ++i)
		{
			Point temp{ 0,0 };
			Point tempA{ 0,0 };

			tempA.x = points[i].x - C.x;
			tempA.y = points[i].y - C.y;

			if (tempA.x != 0 || tempA.y != 0)
			{
				double route;
				route = roundf(sqrt(tempA.x * tempA.x + tempA.y * tempA.y));

				temp.x = (tempA.x / route * cos(radian) - tempA.y / route * sin(radian)) * route;
				temp.y = (tempA.y / route * cos(radian) + tempA.x / route * sin(radian)) * route;

				if (abs(temp.x + C.x) >= window.w / 2)
				{
					isGood = false;
					break;
				}
				else if (abs(temp.y + C.y) >= window.h / 2)
				{
					isGood = false;
					break;
				}
				
			}
		}
		if (isGood)
		{
			for (int i = 0; i < points.size(); ++i)
			{
				Point temp{ 0,0 };
				Point tempA{ 0,0 };

				tempA.x = points[i].x - C.x;
				tempA.y = points[i].y - C.y;

				if (tempA.x != 0 || tempA.y != 0)
				{
					double route;
					route = roundf(sqrt(tempA.x * tempA.x + tempA.y * tempA.y));

					temp.x = (tempA.x / route * cos(radian) - tempA.y / route * sin(radian)) * route;
					temp.y = (tempA.y / route * cos(radian) + tempA.x / route * sin(radian)) * route;

					points[i].x = temp.x + C.x;
					points[i].y = temp.y + C.y;
				}
			}
		}
	}

	void Move(bool isGravity)
	{
		for (int i = 0; i < points.size(); ++i)
		{
			if (abs(points[i].x + V.x * speed) >= window.w / 2)
			{
				V.x *= -1;
				break;
			}
		}	
		for (int i = 0; i < points.size(); ++i)
		{
			if (abs(points[i].y + V.y * speed) >= window.h / 2)
			{
				V.y *= -1;
				break;
			}
		}			

		double gravity = 0.03;

		if (isGravity)
		{
			Point g{ 0, -1};
			V.y += g.y * gravity;

			for (int i = 0; i < points.size(); ++i)
			{
				if (window.h / 2 <= abs(points[i].y) + speed)
				{
					std::cout << "ups\n";
					V.y = 0;
					break;
				}
			}
		}

		Normalize();

		for (int i = 0; i < points.size(); ++i)
		{
			points[i].x += V.x * speed;
			points[i].y += V.y * speed;

			/* 
			//TEST
			int x0 = wWidth / 2;
			int y0 = wHeight / 2;
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
			SDL_RenderDrawLine(ren, x0 + points[i].x, y0 - points[i].y, x0 + points[i].x, y0 + window.h / 2);
			*/
		}
	}
	bool ponce = false;

private:

	SDL_Rect window{ 100, 100, wWidth - 200, wHeight - 200 };

};
/*
void Collision(Square& sq1, Square& sq2)
{
	
	Point c1{ sq1.B.x - abs(sq1.B.x - sq1.A.x) / 2, sq1.B.y + abs(sq1.B.y - sq1.A.y) / 2 };
	Point c2{ sq2.B.x - abs(sq2.B.x - sq2.A.x) / 2, sq2.B.y + SDL_abs(sq2.B.y - sq2.A.y) / 2 };
	
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
*/
void showAxes(int x0, int y0)
{
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderDrawLine(ren, x0, y0 - wHeight / 2, x0, y0 + wHeight / 2);
	SDL_RenderDrawLine(ren, x0 + wWidth / 2, y0, x0 - wWidth / 2, y0);
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

	double speed(0);

	// hard figure

	Point v(7, 5);
	Point A(-170, -80);
	Point B(-130, -60);
	Point C(-170, -10);
	Point D(-220, -50);
	
	std::vector <Point> f;

	f.push_back(A);
	f.push_back(B);
	f.push_back(C);
	f.push_back(D);

	SDL_Color s1{ 255,0,0,255 };

	speed = Speed(500);

	Figure figure(f, v.x, v.y, s1, speed);

	// hard figure

	std::vector <Point> points_circle;
	//circle
	{
		std::cout << "\n";
		int points_count = 12;
		double radius = 40;
		Point center(-10, 100);
		
		for (int i = 0; i < points_count; ++i)
		{
			double rad = (i * 2 * M_PI) / points_count;
			std::cout << rad << "\n";
			double x = center.x + (cos(rad) * radius);
			double y = center.y + (sin(rad) * radius);
			Point point(x, y);
			points_circle.push_back(point);
			std::cout << i << ": { x: " << x << " y: " << y << " }\n";
		}
	}

	Figure circle(points_circle, v.x, v.y, s1, speed);
	//circle

	int x0 = wWidth / 2;
	int y0 = wHeight / 2;

	int timeout = 0;
	double squareTime = SDL_GetTicks64();
	bool isPause = true;
	bool isProcess = true;

	int a = 0.1;
	while (isProcess)
	{
		//LOGIC PART
		
		//events
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
				case SDL_SCANCODE_LEFT: speed - 10 > 0 ? speed -= 10 : speed = speed;
					break;
				case SDL_SCANCODE_RIGHT: speed +=10;
					break;
				}
				break;
			case SDL_QUIT:isProcess = false;
			}

		//Point CenterOfRotate = { 0, 0 };
		Point CenterOfRotate = figure.points[0];
		//If not pause that objects moved
		if (!isPause)
		{
			
			double dist = speed / 10;
			//figure.Rotate(dist, x0, y0, CenterOfRotate);

			figure.Move(false);
			circle.Move(false);
		}

		//Collision(square1, square2);

		//GRAPHIC PART
		showAxes(x0, y0);

		timeout = SDL_GetTicks64() + delay;
		while (SDL_GetTicks64() < timeout)
		{			
			Point CenterOfMass = figure.CenterMass();
			figure.Show(x0, y0);
			circle.Show(x0, y0);
			//SDL_RenderDrawLine(ren, x0 + CenterOfRotate.x, y0 - CenterOfRotate.y, x0 + CenterOfMass.x, y0 - CenterOfMass.y);

			SDL_SetRenderDrawColor(ren, 255, 0, 255, 255);
			SDL_RenderDrawLine(ren, x0 + CenterOfMass.x + figure.V.x * 20, y0 - CenterOfMass.y - figure.V.y * 20, x0 + CenterOfMass.x, y0 - CenterOfMass.y);
		}

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderPresent(ren);
		SDL_RenderClear(ren);

	}

	SDL_Quit();
	return 0;
}