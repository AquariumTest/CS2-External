#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Game.h"
#include "Entity.h"
#include <random>
#include <chrono>

namespace AimControl
{
    
	static int HotKey = VK_LMENU;


	static float AimRange = 600.0f;

    
    static bool AimBotEnabled = false;

    
    static float AimSmoothFactor = 0.1f;

    
    static float AimJitterMagnitude = 0.3f;

    
    static std::default_random_engine jitterRnd(
        static_cast<unsigned int>(
            std::chrono::system_clock::now().time_since_epoch().count()
        )
    );
    static std::uniform_real_distribution<float> jitterDist(-AimJitterMagnitude, AimJitterMagnitude);

	
    inline void CalculateViewAngles(const Vec3& AimPos, const Vec3& LocalPos, float& Yaw, float& Pitch)
    {
        Vec3 OppPos = AimPos - LocalPos;
        float Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

        Yaw = atan2f(OppPos.y, OppPos.x) * 180.0f / static_cast<float>(M_PI);

        if (Distance > 0)
        {
            Pitch = -atanf(OppPos.z / Distance) * 180.0f / static_cast<float>(M_PI);
        }
        else
        {
            Pitch = 0.0f;
        }
    }

    
    inline float SmoothAngle(float currentAngle, float targetAngle, float smoothFactor)
    {
        float delta = targetAngle - currentAngle;

        
        while (delta > 180.0f) delta -= 360.0f;
        while (delta < -180.0f) delta += 360.0f;

        return currentAngle + delta * smoothFactor;
    }

    
    inline void HandleToggleInput()
    {
        static bool wasPressed = false;
        bool isPressed = (GetAsyncKeyState(HotKey) & 0x8000) != 0;

        if (isPressed && !wasPressed)
        {
            AimBotEnabled = !AimBotEnabled;
        }
        wasPressed = isPressed;
    }

 
	inline void AimBot(const CEntity& Local, Vec3 LocalPos,  Vec3 AimPos)
	{
        if (!AimBotEnabled)
            return;

        Vec3 OppPos = AimPos - LocalPos;
        float distanceToTarget = OppPos.Length();
        if (distanceToTarget > AimRange)
            return;

        float targetYaw, targetPitch;
        CalculateViewAngles(AimPos, LocalPos, targetYaw, targetPitch);

        
        float jitterYaw = jitterDist(jitterRnd);
        float jitterPitch = jitterDist(jitterRnd);

        targetYaw += jitterYaw;
        targetPitch += jitterPitch;

        
        float currentYaw, currentPitch;
        gGame.GetViewAngle(currentYaw, currentPitch);

        float smoothYaw = SmoothAngle(currentYaw, targetYaw, AimSmoothFactor);
        float smoothPitch = SmoothAngle(currentPitch, targetPitch, AimSmoothFactor);


        if (smoothPitch > 89.0f) smoothPitch = 89.0f;
        else if (smoothPitch < -89.0f) smoothPitch = -89.0f;

        gGame.SetViewAngle(smoothYaw, smoothPitch);
    }
}
