#ifndef __ADTEMP_H__
#define __ADTEMP_H__
#include "module.h"

/*
 * ��ȡ��ǰ�¶�ֵ 0~999
 */
extern uint16_t get_actulval(void); 

/*
 * ���䡢����
 * state = 0 ����
 * state = 1 ����
 */
extern void ctrl_device(uint8_t state);

#endif
