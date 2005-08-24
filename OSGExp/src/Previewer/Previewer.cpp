/**********************************************************************
 *
 *	FILE:			Previewer.cpp
 *
 *	DESCRIPTION:	Class for previewing osg scenes.
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 28.10.2003
 *
 *	Copyright 2003 Rune Schmidt Jensen
 **********************************************************************/

#include "Previewer.h"
#include <osg/DisplaySettings>
#include <osgDB/ReadFile>

// Static registry to hold previewer class.
std::map <HWND, Previewer *>Previewer::registry;

Previewer::Previewer(){

    _wndX = 100;
    _wndY = 100;
	_wndWidth = 800;
	_wndHeight = 600;
	_exit = false;
	_init = false;

	setDefaults();
//	setDisplayListVisitorMode(osgUtil::DisplayListVisitor::SWITCH_OFF_DISPLAY_LISTS);
}

void Previewer::setWindowSize(int x, int y, int w, int h){
    _wndX = x;
    _wndY = y;
	_wndWidth = w;
	_wndHeight = h;
}

void Previewer::run(){

	if(!_init){
		setViewport(0, 0, _wndWidth, _wndHeight);
		initWindow();
		initTimeAndFrameCount();
		_init = true;
	}

	while(!_exit){

		MSG msg;
		if ( PeekMessage(&msg, _hWnd, 0, 0, PM_REMOVE) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		updateTimeAndFrameCount();

		update();
		cull();
		draw();

		swapBuffers();
	}

	close();
}

void Previewer::initWindow(){

	static TCHAR className[] = TEXT("Previewer");

	// Register the window.
	memset(&_wndClass, 0, sizeof(WNDCLASS));
	_wndClass.style         = CS_OWNDC;
	_wndClass.lpfnWndProc   = (WNDPROC)StaticWindowProc;
	_wndClass.cbClsExtra    = 0;
	_wndClass.cbWndExtra    = 0;
	_wndClass.hInstance     = GetModuleHandle(NULL);
	_wndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	_wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	_wndClass.hbrBackground = NULL;
	_wndClass.lpszMenuName  = NULL;
	_wndClass.lpszClassName = className;
	RegisterClass(&_wndClass);

	// Create window.
	_hWnd = CreateWindow(	className,				// window class name
							className,				// window caption
	                        WS_OVERLAPPEDWINDOW,	// window style
							_wndX,					// initial x position
							_wndY,					// initial y postion
							_wndWidth,				// initital x size
							_wndHeight,				// initial y size
							(HWND)NULL,				// parent window handle
							NULL,					// window menu handle
							GetModuleHandle(NULL),	// program instance handle
							NULL);					// creation parameters

	ShowWindow(_hWnd, SW_SHOW); 
	SetForegroundWindow(_hWnd);
	ShowCursor(TRUE);
  
	_hDC = GetDC(_hWnd);

	// Define pixel format descriptor.
	PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR),								// size
	                              1,															// version
	                              PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,	// support double-buffering
	                              PFD_TYPE_RGBA,												// color type
	                              32,															// prefered color depth
	                              0, 0, 0, 0, 0, 0,												// color bits (ignored)
	                              0,															// no alpha buffer
	                              0,															// alpha bits (ignored)
	                              0,															// no accumulation buffer
	                              0, 0, 0, 0,													// accum bits (ignored)
	                              24,															// depth buffer
	                              0,															// no stencil buffer
	                              0,															// no auxiliary buffers
	                              PFD_MAIN_PLANE,												// main layer
	                              0,															// reserved
	                              0, 0, 0,														// no layer, visible, damage masks
	                            };
															
	// Get the and set pixelformat.
	int pixelFormat = ChoosePixelFormat(_hDC, &pfd);
	SetPixelFormat(_hDC, pixelFormat, &pfd);
  
	// Create OpenGL render context and make it the current.
	_glContext = wglCreateContext(_hDC);
	wglMakeCurrent(_hDC, _glContext);

	// Add this previewer to the registry.
	registry.insert(std::pair<HWND, Previewer *>(_hWnd, this));
}

void Previewer::swapBuffers(){
	::SwapBuffers(_hDC);
}

void Previewer::initTimeAndFrameCount(){
	// set up the time and frame counter.
	_frameNumber = 0;
    _start_tick = _timer.tick();

    if (!_frameStamp) _frameStamp = new osg::FrameStamp;
}

void Previewer::updateTimeAndFrameCount(){
    // set the frame stamp for the new frame.
    double time_since_start = _timer.delta_s(_start_tick,_timer.tick());
    _frameStamp->setFrameNumber(_frameNumber++);
    _frameStamp->setReferenceTime(time_since_start);
	setFrameStamp(_frameStamp.get());
}    

void Previewer::close(){
	this->setSceneData(NULL);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(_glContext);
	::ReleaseDC(_hWnd, _hDC);
	DestroyWindow(_hWnd);
}
/*
void Previewer::setDisplayListVisitorMode(osgUtil::DisplayListVisitor::Mode mode){
	if(dynamic_cast<osgUtil::DisplayListVisitor*>(_initVisitor.get())){
		osgUtil::DisplayListVisitor* dplv = static_cast<osgUtil::DisplayListVisitor*>(_initVisitor.get());
		dplv->setMode(mode);
	}
}*/

LRESULT CALLBACK Previewer::StaticWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

    std::map <HWND, Previewer *>::iterator p;
    p = registry.find( hWnd );
    if( p == registry.end() )
        return DefWindowProc( hWnd, message, wParam, lParam );
    else
        return p->second->handleEvents( hWnd, message, wParam, lParam );
}

LRESULT CALLBACK Previewer::handleEvents(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	switch(message){
		case WM_KEYDOWN:
			switch(wParam){
				case VK_ESCAPE:
					_exit = true;
                    break;
  				default :
				//   MoveCamera(wParam);
				break;
			}
			break;
		case WM_CLOSE:
			_exit = true;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
  
  return 0;
}

