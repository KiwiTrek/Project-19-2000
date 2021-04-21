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

struct Esingfunctions
{
	int Ease(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		timePassed /= time;
		return finish * timePassed + origin;
	}
	int QuintEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		timePassed /= time;
		timePassed--;
		return finish * (timePassed * timePassed * timePassed * timePassed * timePassed + 1) + origin;;
	}
	int CircEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		return -finish * (sqrt(1 - (timePassed /= time) * timePassed) - 1) + origin;
	}
	int BackEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		int pos = 0;
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
	int QuartEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		timePassed /= time;
		timePassed--;
		return -finish * (timePassed * timePassed * timePassed * timePassed - 1) + origin;
	}
	int QuadEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		timePassed /= time;
		return finish * timePassed * timePassed + origin;
	}
	int ExpoEase(float& timePassed, const int& origin, const int& finish, const float& time)
	{
		int pos = 0;
		if ((timePassed /= time / 2) < 1) {
			pos = finish / 2 * pow(2, 10 * (timePassed - 1)) + origin;
		}
		else {
			pos = finish / 2 * (-pow(2, -10 * --timePassed) + 2) + origin;
		}
		return pos;
	}
	int CubicEase(float& timePassed, const int& origin, const int& finish, const float& time)
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
	Esingfunctions functions;

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
				*position = functions.Ease(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::QUINT:
			{
				*position = functions.QuintEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::BACK:
			{
				*position = functions.BackEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::QUART:
			{
				*position = functions.QuartEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::QUAD:
			{
				*position = functions.QuadEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::EXPO:
			{
				*position = functions.ExpoEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::CUBIC:
			{
				*position = functions.CubicEase(timeCounter, initialPosition, finalPosition, totalTime);
			} break;
			case SplineType::CIRC:
			{
				*position = functions.CircEase(timeCounter, initialPosition, finalPosition, totalTime);
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
