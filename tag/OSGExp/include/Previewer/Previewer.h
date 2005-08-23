/**********************************************************************
 *
 *	FILE:			Previewer.h
 *
 *	DESCRIPTION:	Header file for Previewer class.
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 28.10.2003
 *
 *	Copyright 2003 Rune Schmidt Jensen
 **********************************************************************/

#include "Export.h"
#include "windows.h"
#include <osgUtil/SceneView>
//#include <osgUtil/DisplayListVisitor>
#include <osg/Timer>
#include <osg/Framestamp>

class PREVIEWER_EXPORT Previewer: public osgUtil::SceneView{

public:
	Previewer();
	void run();
	void setWindowSize(int x, int y, int w, int h);
//	void setDisplayListVisitorMode(osgUtil::DisplayListVisitor::Mode);


private:
	void initWindow();
	void close();
	void swapBuffers();

	static std::map <HWND, Previewer *>	registry;
	static LRESULT CALLBACK StaticWindowProc(HWND _hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK handleEvents(HWND _hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void initTimeAndFrameCount();
	void updateTimeAndFrameCount();

private:
	bool							_exit;
	bool							_init;

	int								_wndX;
	int								_wndY;
	int								_wndWidth;
	int								_wndHeight;

	unsigned int					_frameNumber;
    osg::Timer						_timer;
    osg::Timer_t					_start_tick;
    osg::ref_ptr<osg::FrameStamp>   _frameStamp;

	WNDCLASS						_wndClass;
	HWND							_hWnd;
	HDC								_hDC;
	HGLRC							_glContext;
};