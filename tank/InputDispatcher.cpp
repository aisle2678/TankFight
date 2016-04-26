#include "InputDispatcher.h"
#include "InputListener.h"

//---------------------------------------------------------------------------
InputDispatcher::InputDispatcher(OIS::Keyboard* kb, OIS::Mouse* ms)
	: mKeyboard(kb), mMouse(ms)
{
	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);
}
//---------------------------------------------------------------------------
InputDispatcher::~InputDispatcher()
{
	mListeners.clear();
}
//---------------------------------------------------------------------------
bool InputDispatcher::keyPressed(const OIS::KeyEvent& ke)
{
	std::for_each(std::begin(mListeners), std::end(mListeners), [&](InputListener *il)
	{
		if (il)
		{
			il->keyPressed(ke);
		}
	});
	return true;
}
//---------------------------------------------------------------------------
bool InputDispatcher::keyReleased(const OIS::KeyEvent& ke)
{
	std::for_each(std::begin(mListeners), std::end(mListeners), [&](InputListener *il)
	{
		if (il)
		{
			il->keyReleased(ke);
		}
	});
	return true;
}
//---------------------------------------------------------------------------
bool InputDispatcher::mouseMoved(const OIS::MouseEvent& me)
{
	std::for_each(std::begin(mListeners), std::end(mListeners), [&](InputListener *il)
	{
		if (il)
		{
			il->mouseMoved(me);
		}
	});
	return true;
}
//---------------------------------------------------------------------------
bool InputDispatcher::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	std::for_each(std::begin(mListeners), std::end(mListeners), [&](InputListener *il)
	{
		if (il)
		{
			il->mousePressed(me, id);
		}
	});
	return true;
}
//---------------------------------------------------------------------------
bool InputDispatcher::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	std::for_each(std::begin(mListeners), std::end(mListeners), [&](InputListener *il)
	{
		if (il)
		{
			il->mouseReleased(me, id);
		}
	});
	return true;
}
//---------------------------------------------------------------------------
void InputDispatcher::addListener(InputListener* il)
{
	mListeners.push_back(il);	
}
