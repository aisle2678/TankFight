#pragma once

class InputListener;

// This class dispatches events to listeners, we don't differentiate
// different input sources and it should be fine for an academic project.
class InputDispatcher : public OIS::KeyListener, public OIS::MouseListener
{
public:

	InputDispatcher(OIS::Keyboard *kb, OIS::Mouse *ms);

	~InputDispatcher();

	bool keyPressed(const OIS::KeyEvent &ke) override;

	bool keyReleased(const OIS::KeyEvent &ke) override;

	bool mouseMoved(const OIS::MouseEvent &ke) override;

	bool mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) override;

	bool mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) override;

	void addListener(InputListener *il);

protected:

	OIS::Keyboard *mKeyboard;
	OIS::Mouse *mMouse;

	// dispatcher never owns the pointer
	std::list<InputListener *> mListeners;
	
};
