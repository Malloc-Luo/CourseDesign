#include "LEDdisplay.h"

/*
 * 注意！当按键有改变数字的时候或者发出重置命令的时候
 * 将这两个变量置1
 */
uint8_t isSetValChanged = 0;
uint8_t isResetRefVal = 0;