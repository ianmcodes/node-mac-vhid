var machid = require("../build/Release/machid");
var util = require("util");

machid.mouseShow();
machid.mouseMoveABS(50,50);
machid.mouseShow();
console.log("should be: 50,50");
var pt = machid.mouseGetCurrentPosition();
console.log("is: %d,%d", pt.x, pt.y);
setTimeout(function() {
  machid.mouseMoveDelta(50,0);
  machid.mouseShow();
  console.log("should be: 100,50");
  pt = machid.mouseGetCurrentPosition();
  console.log("is: %d,%d", pt.x, pt.y);
  setTimeout(function() {
    machid.mouseMoveDelta(0,50);
    machid.mouseShow();
    console.log("should be: 100,100");
    pt = machid.mouseGetCurrentPosition();
    console.log("is: %d,%d", pt.x, pt.y);
    setTimeout(function() {
      machid.mouseMoveDelta(-50,0);
      machid.mouseShow();
      console.log("should be: 50,100");
      pt = machid.mouseGetCurrentPosition();
      console.log("is: %d,%d", pt.x, pt.y);
      setTimeout(function() {
        machid.mouseMoveDelta(0,-50);
        machid.mouseShow();
        console.log("should be: 50,50");
        pt = machid.mouseGetCurrentPosition();
        console.log("is: %d,%d", pt.x, pt.y);
      }, 1000);
    }, 1000);
  }, 1000);
}, 1000);