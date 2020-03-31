#ifndef __SCRIPTINGPARTICLES_H__
#define __SCRIPTINGPARTICLES_H__

#include "BrokenCore.h"

BE_BEGIN_NAMESPACE
class BROKEN_API ScriptingParticles {
public:
	ScriptingParticles();
	~ScriptingParticles();

public:
	void ActivateParticleEmitter() const;
	void DeactivateParticleEmitter() const;

	void PlayParticleEmitter();
	void StopParticleEmitter();
	void SetEmissionRateFromScript(float ms);
	void SetParticlesPerCreationFromScript(int particlesAmount);

	void SetParticleAcceleration(float x, float y, float z);
	void SetParticleVelocityFromScript(float x, float y, float z);
	void SetRandomParticleVelocity(float x, float y, float z);

	void SetParticleLooping(bool active);
	void SetParticleDuration(int duration);
	void SetParticleLifeTime(int ms);

	void SetParticleScaleFromScript(float x, float y);
	void SetRandomParticleScale(float randomFactor);


};
BE_END_NAMESPACE
#endif // __SCRIPTINGPARTICLES_H__