#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2012/04/27 16:50 by Remi Bourreau
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__IHHFX_PUBLIC_H__
#define	__IHHFX_PUBLIC_H__

#if defined(HHFX_PLUGIN_EXPORT)
#	define	HHFX_EXPORT	__declspec(dllexport)
#else
#	define	HHFX_EXPORT	__declspec(dllimport)
#endif

#include <string>
#include <vector>

#include "OgreMovableObject.h"

// forward declares
namespace HellHeaven
{
	class CRendererSubView;
}

class	IHHFXOgre;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//											IHHFXPackExplorer
//////////////////////////////////////////////////////////////////////////////////////////////////////

class HHFX_EXPORT IHHFXPackExplorer
{
public:
	virtual ~IHHFXPackExplorer() {}

	virtual const std::string					&GetPack() const = 0;
	virtual const std::vector<std::string>		&GetEffects() const = 0;
	virtual const std::vector<std::string>		&GetNames() const = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//											IHHFXBase
//////////////////////////////////////////////////////////////////////////////////////////////////////

class HHFX_EXPORT IHHFXBase
{
public:
	virtual ~IHHFXBase() {}

	virtual const IHHFXPackExplorer		&GetHHFXPackExplorer() const = 0;

	virtual bool						LoadPack(const char *pack, bool preloadEffects) = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//											IHHFXScene
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct	SCollisionContext
{
	Ogre::Vector3		m_TraceOrigin;
	Ogre::Vector3		m_TraceDirection;
	float				m_TraceLength;					// length of the trace segment

	float				m_ContactTime;					// distance along the trace ray where the intersection occured
	Ogre::Vector3		m_ContactPoint;
	Ogre::Vector3		m_ContactNormal;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

struct	SCollisionContextSOA
{
	int					m_ElementCount;
	int					m_FilterID;

	const Ogre::Vector4	*m_TraceOrigins;				// origin provided in {x,y,z}
	const Ogre::Vector4	*m_TraceDirectionsAndLengths;	// direction provided in {x,y,z}, length in {w}

	float				*m_ContactTimes;				// distance along the trace ray where the intersection occured
	Ogre::Vector4		*m_ContactPoints;				// contact point expected in {x,y,z}
	Ogre::Vector4		*m_ContactNormals;				// contact point expected in {x,y,z}
	void				**m_ContactObjects;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

class HHFX_EXPORT IHHFXScene
{
public:
	virtual ~IHHFXScene() {}

	virtual IHHFXBase&		GetHHFXBase() const = 0;
	virtual unsigned int	GetParticleCount() const = 0;
	virtual float			GetSimulationSpeed() const = 0;
	virtual void			SetSimulationSpeed(float speed) = 0;

	// by default, world scale is set to 1.0f
	virtual float			GetWorldScale() const = 0;
	virtual void			SetWorldScale(float scale) = 0;

	virtual void			Update(float timeSinceLastFrame) = 0;
	virtual void			Render(HellHeaven::CRendererSubView &view, const Ogre::Vector3 &cam) = 0;
	virtual void			Clear() = 0;

	// note: if the two collision callbacks are set, both will be called
	virtual void			SetCollisionCallback(void *arg, bool (*cbTrace)(void *arg, SCollisionContext &collisionContext)) = 0;
	virtual void			SetCollisionCallback(void *arg, void (*cbTrace)(void *arg, const SCollisionContextSOA &collisionContext)) = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//											IHHFXListener
//////////////////////////////////////////////////////////////////////////////////////////////////////

class IHHFXListener
{
public:
	virtual	~IHHFXListener() {}

	virtual void		OnFXStarted(IHHFXOgre *obj) = 0;
	virtual void		OnFXStopped(IHHFXOgre *obj) = 0;
	//virtual void		OnFXDestroyed(IHHFXOgre *obj) = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//											IHHFXOgre
//////////////////////////////////////////////////////////////////////////////////////////////////////

enum	EHHFXBuiltinTypes
{
	HHFX_Float = 0,
	HHFX_Float2,
	HHFX_Float3,
	HHFX_Float4,
	HHFX_Int,
	HHFX_Int2,
	HHFX_Int3,
	HHFX_Int4,

	__MaxHHFXTypes
};

template<typename _Type>	struct	THHFXBuiltinType				{};
template<>					struct	THHFXBuiltinType<float>			{ static const EHHFXBuiltinTypes TypeID = HHFX_Float; };
template<>					struct	THHFXBuiltinType<Ogre::Vector2>	{ static const EHHFXBuiltinTypes TypeID = HHFX_Float2; };
template<>					struct	THHFXBuiltinType<Ogre::Vector3>	{ static const EHHFXBuiltinTypes TypeID = HHFX_Float3; };
template<>					struct	THHFXBuiltinType<Ogre::Vector4>	{ static const EHHFXBuiltinTypes TypeID = HHFX_Float4; };
template<>					struct	THHFXBuiltinType<int>			{ static const EHHFXBuiltinTypes TypeID = HHFX_Int; };

//////////////////////////////////////////////////////////////////////////////////////////////////////

class HHFX_EXPORT IHHFXOgre : public Ogre::MovableObject
{
public:
	IHHFXOgre(const Ogre::String& objectSceneName) : MovableObject(objectSceneName) {}

	virtual ~IHHFXOgre() {}

	virtual IHHFXBase		&GetHHFXBase() const = 0;
	virtual IHHFXScene		&GetHHFXScene() const = 0;
	virtual void			SetFXListener(IHHFXListener *fxListener) = 0;

	// tell if the effect is still active.
	// if the effect is inactive, you will not be able to call the HHFx methods.
	// if you experience some crash in you application when trying to call HHFx methods, 
	// check this before.
	virtual bool			IsFXActive() const = 0;

	virtual bool			LoadFX(const char *fx, bool run) = 0;
	virtual bool			RunFX() = 0;
	virtual bool			StopFX() = 0;

	template<typename _Type>
	bool					SetAttribute(const char *name, const _Type &value) { return _SetAttributeImpl(name, &value, THHFXBuiltinType<_Type>::TypeID); }

	virtual const char		*GetPath() const = 0;

	virtual bool			SetAttribute(const char *name, const void *valuePtr, EHHFXBuiltinTypes valueType) = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// __IHHFX_PUBLIC_H__
