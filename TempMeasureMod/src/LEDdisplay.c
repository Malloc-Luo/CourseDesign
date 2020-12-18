#include "LEDdisplay.h"

/*
 * 注意！当按键有改变数字的时候或者发出重置命令的时候
 * 将这两个变量置1
 */
bit isSetValChanged = 0;
bit isResetRefVal = 0;

