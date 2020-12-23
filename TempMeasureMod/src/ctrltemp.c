#include <math.h>
#include "module.h"
#include "ADtemp.h"

/*
 * 设定值与实际值的误差
 */
static int16_t xdata error = 0;

void Ctrl_Temperture(int16_t setval, int16_t actulval)
{
    error = setval - actulval;
    /*
     * 误差小于 1°C 时关闭设备
     */
    if (abs(error) < 20)
    {
        ctrl_device(CLOSE);
    }
    else
    {
        if (error < 0)
        {
            ctrl_device(COLD);
        }
        else
        {
            ctrl_device(HEAT);
        }
    }
}