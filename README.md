Virtual Human Input Device for Node.js
======================================

Node addon that allows you to control the mouse and keyboard on a Mac through a virtual human input device.

~~Currently only mouse movement has been implemented.~~

Mouse and keyboad are both now implemented!

API
---

 * mouseMoveABS(x,y)
 * mouseMoveDelta(dx,dy)
 * mouseShow()
 * mouseGetCurrentPosition() -> { x: Number, y: Number }
 * keyDown(code)
 * keyUp(code)