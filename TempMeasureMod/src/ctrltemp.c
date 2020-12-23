#include <math.h>
#include "module.h"
#include "ADtemp.h"

/*
 * �趨ֵ��ʵ��ֵ�����
 */
static int16_t xdata error = 0;

void Ctrl_Temperture(int16_t setval, int16_t actulval)
{
    error = setval - actulval;
    /*
     * ���С�� 1��C ʱ�ر��豸
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