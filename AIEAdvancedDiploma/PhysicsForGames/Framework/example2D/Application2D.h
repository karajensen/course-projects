#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Audio.h"

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_texture;
	aie::Font*			m_font;
	aie::Audio*			m_audio;

	aie::Texture*		m_shipTexture;
	float				m_shipX, m_shipY;
	float				m_shipSpeed;

	float m_timer;
};