//
//  main.cpp
//  node-mac-hid
//
//  Created by Ian McCall on 2/22/14.
//  See LICENSE.txt
//

#include <CoreGraphics/CoreGraphics.h>
#include <node.h>
#include <v8.h>

using namespace v8;

CGEventSourceRef evtSrc = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

///////////////////////////////
/*****************************
 * Mouse getters and setters *
 *****************************/
CGRect bounds = CGDisplayBounds(kCGDirectMainDisplay);
double maxY = bounds.size.height - 1;
double maxX = bounds.size.width - 1;

void setMousePosition(double x, double y) {
	// sanity check will go here
	if (x < 0) {
		x = 0;
	} else if (x > maxX) {
		x = maxX;
	}
	if (y < 0) {
		y = 0;
	} else if (y > maxY) {
		y = maxY;
	}
	CGEventRef evt = CGEventCreateMouseEvent(evtSrc, kCGEventMouseMoved, CGPointMake(x, y), kCGMouseButtonLeft);
	CGEventPost(kCGSessionEventTap, evt);
	CFRelease(evt);
}

CGPoint getMousePosition() {
	CGEventRef evt = CGEventCreate(NULL);
	CGPoint pt = CGEventGetLocation(evt);
	CFRelease(evt);
	return pt;
}

void showMouse() {
	CGDisplayShowCursor(kCGDirectMainDisplay);
}

void button(uint32_t btn, bool up) {
	uint32_t evtType = 0;
	if (btn == kCGMouseButtonLeft) {
		if (up) {
			evtType = kCGEventLeftMouseUp;
		} else {
			evtType = kCGEventLeftMouseDown;
		}
	} else if (btn == kCGMouseButtonRight) {
		if (up) {
			evtType = kCGEventRightMouseUp;
		} else {
			evtType = kCGEventRightMouseDown;
		}
	}
	CGPoint pt = getMousePosition();
	CGEventRef evt = CGEventCreateMouseEvent(evtSrc, evtType, pt, btn);
	CGEventPost(kCGHIDEventTap, evt);
	CFRelease(evt);
}
///////////////////////////////

///////////////////////////////
/************
 * Keyboard *
 ************/
void key(uint32_t code, bool down) {
	CGEventRef evt = CGEventCreateKeyboardEvent(evtSrc, (CGKeyCode)code, down);
	CGEventPost(kCGHIDEventTap, evt);
	CFRelease(evt);
}
///////////////////////////////

///////////////////////////////
/***********************
 * JavaScript handlers *
 ***********************/
/*
 * Number x
 * Number y
 */
Handle<Value> mouseMoveABS(const Arguments& args) {
	HandleScope scope;
	
	if (args.Length() < 2) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}
	
	if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Undefined());
	}
	
	setMousePosition(args[0]->NumberValue(), args[1]->NumberValue());
	return scope.Close(Undefined());
}

/*
 * Number dx
 * Number dy
 */
Handle<Value> mouseMoveDelta(const Arguments& args) {
	HandleScope scope;
	double dx, dy, x, y;
	CGPoint pt = getMousePosition();
	
	if (args.Length() < 2) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}
	
	if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Undefined());
	}
	
	dx = args[0]->NumberValue();
	dy = args[1]->NumberValue();
	x = pt.x + dx;
	y = pt.y + dy;
	setMousePosition(x, y);
	
	return scope.Close(Undefined());
}

/*
 * No args
 */
Handle<Value> mouseGetCurrentPosition(const Arguments& args) {
	HandleScope scope;
	Local<Object> obj = Object::New();
	CGPoint pt = getMousePosition();
	obj->Set(String::NewSymbol("x"), Number::New(pt.x));
	obj->Set(String::NewSymbol("y"), Number::New(pt.y));
	return scope.Close(obj);
}

/*
 * No args
 */
Handle<Value> mouseShow(const Arguments& args) {
	HandleScope scope;
	showMouse();
	return scope.Close(Undefined());
}

/*
 * Number button
 *   Left -> 0
 *  Right -> 1
 */
Handle<Value> mouseBtnDown(const Arguments& args) {
	HandleScope scope;
	uint32_t btn = 0;
	if (args.Length() > 0 && args[0]->IsNumber()) {
		btn = args[0]->Int32Value();
	}
	button(btn, false);
	return scope.Close(Undefined());
}

/*
 * Number button
 *   Left -> 0
 *  Right -> 1
 */
Handle<Value> mouseBtnUp(const Arguments& args) {
	HandleScope scope;
	uint32_t btn = 0;
	if (args.Length() > 0 && args[0]->IsNumber()) {
		btn = args[0]->Int32Value();
	}
	button(btn, true);
	return scope.Close(Undefined());
}

Handle<Value> keyDown(const Arguments& args) {
	HandleScope scope;
	uint32_t code = 0;
	if (args.Length() < 1 && !args[0]->IsNumber()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Undefined());
	}
	code = args[0]->Int32Value();
	key(code, true);
	return scope.Close(Undefined());
}


Handle<Value> keyUp(const Arguments& args) {
	HandleScope scope;
	uint32_t code = 0;
	if (args.Length() < 1 && !args[0]->IsNumber()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Undefined());
	}
	code = args[0]->Int32Value();
	key(code, false);
	return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("mouseMoveDelta"),
				 FunctionTemplate::New(mouseMoveDelta)->GetFunction());
	exports->Set(String::NewSymbol("mouseMoveABS"),
				 FunctionTemplate::New(mouseMoveABS)->GetFunction());
	exports->Set(String::NewSymbol("mouseGetCurrentPosition"),
				 FunctionTemplate::New(mouseGetCurrentPosition)->GetFunction());
	exports->Set(String::NewSymbol("mouseShow"),
				 FunctionTemplate::New(mouseShow)->GetFunction());
	exports->Set(String::NewSymbol("mouseBtnDown"),
				 FunctionTemplate::New(mouseBtnDown)->GetFunction());
	exports->Set(String::NewSymbol("mouseBtnUp"),
				 FunctionTemplate::New(mouseBtnUp)->GetFunction());
	exports->Set(String::NewSymbol("keyDown"),
				 FunctionTemplate::New(keyDown)->GetFunction());
	exports->Set(String::NewSymbol("keyUp"),
				 FunctionTemplate::New(keyUp)->GetFunction());
}

NODE_MODULE(machid, init)
