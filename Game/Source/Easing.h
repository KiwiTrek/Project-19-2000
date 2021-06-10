#pragma once
#include "Log.h"
#include <list>
#include "SDL/include/SDL_timer.h"

struct SDL_Texture;
struct SDL_Rect;

enum class SplineType
{
	EASE,
	EXPO,
	CIRC,
	QUINT,
	QUART,
	QUAD,
	BACK,
	ELASTIC,
	CUBIC,
	NONE
};

struct Easingfunctions
{
	float Ease(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		timePassed /= time;
		return finish * timePassed + origin;
	}
	float QuintEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		timePassed /= time;
		timePassed--;
		return finish * (timePassed * timePassed * timePassed * timePassed * timePassed + 1) + origin;;
	}
	float CircEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		return -finish * (sqrt(1 - (timePassed /= time) * timePassed) - 1) + origin;
	}
	float BackEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		float pos = 0.0f;
		float s = 1.0f;
		if ((timePassed /= time / 2) < 1) {
			pos = finish / 2 * (timePassed * timePassed * (((s *= (0.9f)) + 1) * timePassed - s)) + origin;
		}
		else {
			float postFix = timePassed -= 2;
			pos = finish / 2 * ((postFix)*timePassed * (((s *= (0.9f)) + 1) * timePassed + s) + 2) + origin;
		}
		return pos;
	}
	float QuartEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		timePassed /= time;
		timePassed--;
		return -finish * (timePassed * timePassed * timePassed * timePassed - 1) + origin;
	}
	float QuadEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		timePassed /= time;
		return finish * timePassed * timePassed + origin;
	}
	float ExpoEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		float pos = 0.0f;
		if ((timePassed /= time / 2) < 1) {
			pos = finish / 2 * pow(2, 10 * (timePassed - 1)) + origin;
		}
		else {
			pos = finish / 2 * (-pow(2, -10 * --timePassed) + 2) + origin;
		}
		return pos;
	}
	float CubicEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		timePassed /= time;
		timePassed--;
		return finish * (timePassed * timePassed * timePassed + 1) + origin;
	}

};

struct SplineInfo {

	int* position = nullptr;
	int initialPosition = 0;
	int finalPosition = 0;

	float totalTime = 0.0F;
	float timePassed = 0.0F;

	SplineType type;
	Easingfunctions functions;

	bool Update(float dt)
	{
		bool ret = true;

		float timeCounter = SDL_GetTicks() - timePassed;

		if (timeCounter < totalTime)
		{
			switch (type)
			{
			case SplineType::EASE:
			{
				*position = (int)functions.Ease(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::QUINT:
			{
				*position = (int)functions.QuintEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::BACK:
			{
				*position = (int)functions.BackEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::QUART:
			{
				*position = (int)functions.QuartEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::QUAD:
			{
				*position = (int)functions.QuadEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::EXPO:
			{
				*position = (int)functions.ExpoEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::CUBIC:
			{
				*position = (int)functions.CubicEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::CIRC:
			{
				*position = (int)functions.CircEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			default:
				LOG("No valid SplineType");
				break;
			}
		}
		else
			ret = false;

		return ret;
	};

	SplineInfo(int* position, const int& finalPosition, const float& totalTime, const SplineType& type) {

		this->position = position;
		this->initialPosition = *position;
		this->finalPosition = finalPosition - *position;

		this->type = type;
		this->totalTime = totalTime;
		timePassed = SDL_GetTicks();
	}
};
