//
//  main.cpp
//  node-mac-hid
//
//  Created by Ian McCall on 2/22/14.
//  Copyright (c) 2014 Ian McCall. All rights reserved.
//

#include <CoreGraphics/CoreGraphics.h>
//#include <AppKit/AppKit.h>
#include <node.h>
#include <v8.h>

using namespace v8;

///////////////////////////////
/*****************************
 * Mouse getters and setters *
 *****************************/
CGEventSourceRef evtSrc = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

void setMousePosition(double x, double y) {
	// sanity check will go here
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

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("mouseMoveDelta"),
				 FunctionTemplate::New(mouseMoveDelta)->GetFunction());
	exports->Set(String::NewSymbol("mouseMoveABS"),
				 FunctionTemplate::New(mouseMoveABS)->GetFunction());
	exports->Set(String::NewSymbol("mouseGetCurrentPosition"),
				 FunctionTemplate::New(mouseGetCurrentPosition)->GetFunction());
	exports->Set(String::NewSymbol("mouseShow"),
				 FunctionTemplate::New(mouseShow)->GetFunction());
}

NODE_MODULE(machid, init)
