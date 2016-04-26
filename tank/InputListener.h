#pragma once

class InputDispatcher;
class World;

class InputListener
{
	// well, only input dispatcher can access those methods
	// subclasses that inheriting this base class should 
	// keep it protected
	friend InputDispatcher;

protected:

	virtual void keyPressed(const OIS::KeyEvent &ke) {}

	virtual void keyReleased(const OIS::KeyEvent &ke) {}

	virtual void mouseMoved(const OIS::MouseEvent &ke) {}

	virtual void mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) {}

	virtual void mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) {}

	virtual ~InputListener() {}
};