## 下位机硬件资源分配及软件架构

### 一、硬件资源分配

| I/O                        | 功能分配         |
| -------------------------- | ---------------- |
| P1.0, P1.1, P1.2, P1.3     | 制冷制热设备控制 |
| P0.0, P0.1, P0.2, P0.3     | 数码管及键盘     |
| P2, P0.4, P0.5, P3.2, P3.3 | AD转换接口       |
| P3.0, P3.1, T1(定时器)     | 串口             |

### 二、软件架构及接口

```
src-->
    |--> main.c
    |--> bluetooth.c
    |--> schedul.c
    |--> LEDdisplay.c
    |--> ADtemp.c
inc-->
    |--> module.h
    |--> bluetooth.h
    |--> LEDdisplay.h
    |--> ADtemp.h
```

#### 1. 总体结构

所有程序通过一个基于T0的调度器进行带调度，T0初始化在主函数中，使能中断，中断周期50ms。全局变量：

```c
/* 系统时钟计数，50ms */
static uint16_t xdata systick = 0;
```

在定时器中断中计数并以此进行调度

```c
void timer0() interrupt 1 using 1
{
    systick++;
    /*系统调度*/
    system_scheduler();
}
```

所有模块的程序在调度任务中运行。调度任务一共有四个：

```c
// 10Hz频率，也就是100ms执行一次，其它同理
extern void Task_10Hz_1();
extern void Task_10Hz_2();
extern void Task_2Hz();
extern void Task_1Hz();
```

**注意！！不要在任务里使用超过50ms的延时函数**

函数定义在`schedul.c`中，只需要把你的程序接口放在上述四个函数中（有具体要求，不一定要你放，看下文）

所有的硬件初始化、工作寄存器初始配置都要放在`main.c`中`hardware_init()`函数中。

#### 2. 接口定义

每个模块的负责人员只需要实现一个接口就行了，具体怎么实现由自己负责，只要性能不拉跨就行！

`module.h`中放了使用到的全局变量，在测试模块时需要**包含此文件到你的测试工程**

```c
/**
 * 温控模块的温度
 * 将获取到的温度 * 10后转换成16位整形
 */
extern uint16_t ModTemperture;
/*
 * 设定的温度值
 */
extern uint16_t SetTemperture;
```

##### 1> A/D转换温度获取及加热制冷控制

功能实现放在`ADtemp.h`和`ADtemp.c`中，需要实现俩个功能：

*   温度获取
*   制冷制热设备控制接口

```c
/*
 * 获取当前温度值 0~999
 */
extern uint16_t get_actulval(void); 
/*
 * 制冷、制热，只要用户调用这个函数就能实现制冷和制热
 * state = 0 制冷
 * state = 1 制热
 */
extern void ctrl_device(uint8_t state);
```

##### 2> 数码管显示及按键处理

功能实现放在`LEDdisplay.h`与`LEDdisplay.c`中，需要实现的功能有：

*   显示设定值和当前值
*   处理和获取当前设定值

```c
/*
 * setval: 设定温度值
 * actulval: 实际温度值
 */
extern void LED_display(uint16_t setval, uint16_t actulval);
/*
 * 获取设定温度值
 */
extern uint16_t get_setval(void);
```

##### 3> 蓝牙串口

功能实现放在`bluetooth.h`和`bluetooth.c`中，需要实现的功能：

*   接收上位机蓝牙串口指令
*   向上位机蓝牙串口反馈数据

```c
void send_data(uint16_t temperture, uint8_t instruction);
void recv_data(uint16_t *, uint8_t *);
```

