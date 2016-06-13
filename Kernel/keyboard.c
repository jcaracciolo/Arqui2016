#include "include/videoDriver.h"

static int ctrlPressed = -1;
static int shiftPressed = -1;
static int altPressed = -1;
static int mapSize = 90;
//
//void printKey() {
//	int keyCode=-1;
//
//	// Key Pressed
//	if (keyCode >= 0 && keyCode < mapSize) {
//		if (keyCode == 28) {							// Enter
//			printNewLine();
//		} else if (keyCode == 1) {
//			clear();
//		} else if (keyCode == 14) {						// Backspace
//			backspace();
//		} else if (keyCode == 42 || keyCode == 54) {	// L/R Shift
//			shiftPressed *= -1;
//		} else if (keyCode == 58) {						// Bloq Mayus
//			shiftPressed *= -1;
//		} else if (keyCode == 29) {						// Ctrl
//			ctrlPressed *= -1;
//		} else if (keyCode == 72) {						// Up Arrow
//			moveCursorUp();
//		} else if (keyCode == 80) {						// Down Arrow
//			moveCursorDown();
//		} else if (keyCode == 75) {						// Left Arrow
//			moveCursorLeft();
//		} else if (keyCode == 77) {						// Right Arrow
//			moveCursorRight();
//		} else if (keyCode == 83) {						// Supr
//			supr();
//		} else {										// Other
//			if (shiftPressed == 1) {
//				print(SHIFT_KEYS_VALUES[keyCode], 0x07);
//			} else {
//				print(KEYS_VALUES[keyCode], 0x07);
//			}
//		}
//	// Key Released
//	} else {
//		if (keyCode == 170 || keyCode == 182) {			// L/R Shift
//			shiftPressed *= -1;
//		}
//		if (keyCode == 157) {							// Ctrl
//			ctrlPressed *= -1;
//		}
//	}
//
//}
