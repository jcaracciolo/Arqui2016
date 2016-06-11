#include "videoDriver.h"

const char* KEYS_VALUES[] = {"", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", 			// 0 - 10
				"0", "'", "¿", "BACKSPACE", "    ", "q", "w", "e", "r", "t",// 11 - 20
			  	"y", "u", "i", "o", "p", "´", "+", "", "", "a",				// 21 - 30
				"s", "d", "f", "g", "h", "j", "k", "l", "", "{", 			// 31 - 40
			  	"|", "LSHIFT", "}", "z", "x", "c", "v", "b", "n", "m",  	// 41 - 50
			  	",", ".", "-", "", "*", "ALT", " ", "", "", "",				// 51 - 60
			  	"", "", "", "", "", "", "", "", "", "", 					// 61 - 70
			  	"7", "UARR", "9", "-", "LARR", "5", "RARR", "+", "1", "2",  // 71 - 80
			  	"3", "0", ".", "", "", "", "", "", "", ""}; 				// 81 - 90

const char* SHIFT_KEYS_VALUES[] = {"", "ESC", "!", "\"", "#", "$", "%", "&", "/", "(", ")", 	// 0 - 10
					"=", "?", "¡", "BACKSPACE", "    ", "Q", "W", "E", "R", "T",// 11 - 20
				  	"Y", "U", "I", "O", "P", "¨", "*", "", "", "A",				// 21 - 30
					"S", "D", "F", "G", "H", "J", "K", "L", "", "[", 			// 31 - 40
				  	"°", "LSHIFT", "]", "Z", "X", "C", "V", "B", "N", "M",  		// 41 - 50
				  	";", ":", "_", "", "*", "ALT", " ", "", "", "",				// 51 - 60
				  	"", "", "", "", "", "", "", "", "", "", 					// 61 - 70
				  	"7", "8", "9", "-", "4", "5", "6", "+", "1", "2", 			// 71 - 80
				  	"3", "0", ".", "", "", "", "", "", "", ""}; 				// 81 - 90

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
