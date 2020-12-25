#include <reg51.h>
#include "intrins.h"
#include "LCDdisplay.h"

/*
 * 注意！当按键有改变数字的时候或者发出重置命令的时候
 * 将这两个变量置1
 */
bit isSetValChanged = 0;
bit isResetRefVal = 0;
bit isReset = 0;
bit isShowRef = 0;
bit isBelow = 0;

bit isSetNegative=0;
bit isActNegative=0;
bit isRefNegative=0;


sbit SCL = P2 ^ 0; //LCD
sbit SDA = P2 ^ 1;

sbit S1 = P1 ^ 0; //遥控器按键
sbit S2 = P1 ^ 1;
sbit S3 = P1 ^ 2;
sbit S4 = P1 ^ 3;
sbit S5 = P1 ^ 4;
sbit S6 = P1 ^ 5;
sbit S7 = P1 ^ 6;
sbit S8 = P1 ^ 7;

char ADDR = 0x4E;    // PCF8574  T  模块的地址码

uint8_t degree_centi[] = {0x16, 0x09, 0x08, 0x08, 0x08, 0x09, 0x06, 0x00}; //自定义字符（℃）
uint8_t signal[] = {0x01, 0x01, 0x05, 0x05, 0x15, 0x15, 0x15, 0x00};//信号图

uint8_t code str_set[] = "Preset";
uint8_t code str_actul[] = "Actual";

//***************************** 延时 y  ms ***********************************************

void delay1(int y)   //
{

    while(y--)
    {
        unsigned char a, b, c;

        for(c = 1; c > 0; c--)
        {
            for(b = 70; b > 0; b--)
            {
                for(a = 2; a > 0; a--);
            }
        }
    }
}


//******************************** IIC 串口开始 ********************************************

void IIC_start(void)
{
    SDA = 1;
    _nop_();
    SCL = 1;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SDA = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SCL = 0;
}


//********************************** IIC 串口写1个字节 ******************************************

void IIC_writeByte(char temp)
{
    char i;

    for(i = 0; i < 8; i++)
    {
        SDA = (bit)(temp & 0x80) ; // 根据规定1602的数据最高位必须为  1
        temp <<= 1;
        _nop_();
        _nop_();
        SCL = 1;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        SCL = 0;
    }

    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SDA = 1;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SCL = 1;
    _nop_();
    _nop_();
    _nop_();

    while(SDA);

    _nop_();
    SCL = 0;
}


//******************************** 1602写命令 ********************************************

void LCD_write_command(char comm)
{
    char tmp;
    IIC_start();          // 串口开始
    IIC_writeByte(ADDR);  // 先选PCF 8574T 的地址  （应该是相当于选中的意思吧）

    tmp = comm & 0xF0;    // 与0xf0 应该是取第四位的意思吧
    tmp |= 0x0C;         //保留高4位为指令的高四位，低四位为   RS = 0, RW = 0, EN = 1
    IIC_writeByte(tmp);  //从串口送出
    delay1(5);
    tmp &= 0xFB;        //Make EN = 0
    IIC_writeByte(tmp);

    tmp = (comm & 0x0F) << 4 ;  //将指令的低四位 送到高位置保存
    tmp |= 0x0C;        //RS = 0, RW = 0, EN = 1
    IIC_writeByte(tmp);
    delay1(5);
    tmp &= 0xFB; // Make EN = 0
    IIC_writeByte(tmp);

}
//******************************** 1602写数据 ********************************************


void LCD_write_data(char data1)
{
    char tmp;
    IIC_start();
    IIC_writeByte(ADDR);   // 先选PCF 8574T 的地址  （应该是相当于选中的意思吧）

    tmp = data1 & 0xF0;
    tmp |= 0x0D; //RS = 0, RW = 0, EN = 1
    IIC_writeByte(tmp);
    delay1(5);
    tmp &= 0xFB; //Make EN = 0
    IIC_writeByte(tmp);

    tmp = (data1 & 0x0F) << 4 ;
    tmp |= 0x0D; //RS = 0, RW = 0, EN = 1
    IIC_writeByte(tmp);
    delay1(5);
    tmp &= 0xFB ; // Make EN = 0
    IIC_writeByte(tmp);
}


//******************************** 1602初始化 ********************************************


void Init_Lcd(void)
{
    LCD_write_command(0x33); //将8位总线转为4位总线
    delay1(5) ;
    LCD_write_command(0x32); //
    delay1(5) ;
    LCD_write_command(0x28); // 4位数据线，显示2行，5*7点阵字符  ！如果是0x38  则为8位数据线，显示2行，5*7点阵字符
    delay1(5) ;
    LCD_write_command(0x0C); // 开显示，关闭光标，不闪烁
    delay1(5) ;
    LCD_write_command(0x06); // 设定输入方式，增量不位移
    delay1(5) ;
    LCD_write_command(0x01); // 清屏
    delay1(5) ;
}



//*************************************** 在指定位置显示字符串 *************************************

void Write_LCD(int x, int y, char *str)
{
    char addr;

    if( x < 0)
    {
        x = 0;
    }

    if(x > 15)
    {
        x = 15;
    }

    if(y < 0)
    {
        y = 0;
    }

    if(y > 1)
    {
        y = 1;
    }

    addr = 0x80 + 0x40 * y + x;   // Move cursor  移动光标
    LCD_write_command(addr);

    while (*str)
    {
        LCD_write_data(*str++);
    }
}

//*************按指定位置显示一个字符(针对1602液晶)-用在温度显示*********************

void DisplayOneChar(uint8_t X, uint8_t Y, uint8_t DData)
{
    Y &= 0x1;
    X &= 0xF;                 //限制X不能大于15，Y不能大于1

    if (Y) X |= 0x40;        //当要显示第二行时地址码+0x40;

    X |= 0x80;               // 算出指令码
    LCD_write_command(X);    //这里不检测忙信号，发送地址码
    LCD_write_data(DData);
}


//把设定字存入CGRAM

void setchar()
{
    uint8_t i,j;
    LCD_write_command(0x40); // 设置第一个字的起始地址
    for(i = 0; i < 8; i++)
    {
        LCD_write_data(degree_centi[i]);
    }
		
		LCD_write_command(0x48);//设置第二个字的起始地址
		for(j = 0; j < 8; j++)
    {
        LCD_write_data(signal[j]);
    }
		
}


//*************************************************************************************************
//LCD显示 实际值、设定值
void LCD_display(int16_t setval, int16_t actulval)
{
    uint8_t setval_1000,setval_100, setval_10, setval_1;
    uint8_t actulval_1000,actulval_100, actulval_10, actulval_1;
		
	
		if (setval < 0)
		{
			isSetNegative = 1;
			setval = -setval;
			DisplayOneChar(9, 1, '-');
		}
		else
		{
			if (isSetNegative == 1)
			{
				LCD_clear();
				isSetNegative = 0;
			}
		}
		
		if (actulval < 0)
		{
			isActNegative = 1;
			actulval = -actulval;
			DisplayOneChar(0, 1, '-');
		}
		else
		{
			if (isActNegative == 1)
			{
				LCD_clear();
				isActNegative = 0;
			}
		}
		
    setchar();
    actulval_1000 = actulval / 1000;
    actulval_100 = (actulval % 1000) / 100;
    actulval_10 = (actulval % 100) / 10;
    actulval_1 = actulval % 10;
    
		setval_1000 = setval / 1000;
    setval_100 = (setval % 1000) / 100;
    setval_10 = (setval % 100) / 10;
    setval_1 = setval % 10;

    Write_LCD(1, 0, str_actul);
    Write_LCD(10, 0, str_set);
   
    DisplayOneChar(8, 0, '|');
    DisplayOneChar(8, 1, '|');
    setchar();


    DisplayOneChar(1, 1, actulval_1000 + '0');
    DisplayOneChar(2, 1, actulval_100 + '0');
		DisplayOneChar(3, 1, actulval_10 + '0');
    DisplayOneChar(4, 1, '.');
    DisplayOneChar(5, 1, actulval_1 + '0');
    DisplayOneChar(6, 1, 0x00);

    DisplayOneChar(10, 1, setval_1000 + '0');
    DisplayOneChar(11, 1, setval_100 + '0');
		DisplayOneChar(12, 1, setval_10 + '0');
    DisplayOneChar(13, 1, '.');
    DisplayOneChar(14, 1, setval_1 + '0');
    DisplayOneChar(15, 1, 0x00);

}

//*************************************************************************************************

//遥控器按键设置

void key_set()
{
    if (S1 == 0)   //重置最低参考值
    {
        isReset = 1;
        isResetRefVal = 1;
    }

    if (S2 == 0)  //展示温度设定范围
    {
        isShowRef = 1;
    }

    //***对设定温度加减***
    if (S7 == 0)
        SetTemperture += 100;

    if (S8 == 0)
        SetTemperture -= 100;

    if (S5 == 0)
        SetTemperture += 10;

    if (S6 == 0)
        SetTemperture -= 10;

    if (S3 == 0)
        SetTemperture += 1;

    if (S4 == 0)
        SetTemperture -= 1;


    //设定温度超出参考范围时
    if (SetTemperture > MAX_TEMP)
    {
        SetTemperture = MAX_TEMP;
        isBelow = 1;
    }

    if (SetTemperture < RefTemperture)
    {
       SetTemperture = RefTemperture;
        isBelow = 1;
    }

    //改变温度设定值
    if((P1 & 0x3f) != 0x3f)
    {
        isSetValChanged = 1;
    }

}

//展示重置成功
void display_reset()
{
    uint8_t str_reset0[] = "Reset";
    uint8_t str_reset1[] = "Successfully";
	
		
      LCD_clear();//清屏
      Write_LCD(5, 0, str_reset0);
			Write_LCD(2, 1, str_reset1);
			delay1(1000);
      LCD_clear();
}

//展示温度设定参考范围
void display_reftemp(int16_t reftemp)
{
    uint8_t str_minreftemp[] = "MINTemp:";
    uint8_t str_maxreftemp[] = "MAXTemp:";
    uint8_t reftemp_100,  reftemp_10, reftemp_1;
    setchar();

		LCD_clear();
    Write_LCD(0, 0, str_minreftemp);
    Write_LCD(0, 1, str_maxreftemp);
		
		if (reftemp < 0)
		{ 
			reftemp = -reftemp;
			isRefNegative = 1;
			DisplayOneChar(10, 0, '-');
		}
		else
		{
			if(isRefNegative == 1)
			{
				LCD_clear();
				isRefNegative = 0;
			}
		}
			
    reftemp_100 = reftemp / 100;
    reftemp_10 = (reftemp % 100) / 10;
    reftemp_1 = reftemp % 10;


    DisplayOneChar(11, 0, reftemp_100 + '0');
    DisplayOneChar(12, 0, reftemp_10 + '0');
    DisplayOneChar(13, 0, '.');
    DisplayOneChar(14, 0, reftemp_1 + '0');
    DisplayOneChar(15, 0, 0x00);

    DisplayOneChar(10, 1, '3');
    DisplayOneChar(11, 1, '0');
		DisplayOneChar(12, 1, '0');
    DisplayOneChar(13, 1, '.');
    DisplayOneChar(14, 1, '0');
    DisplayOneChar(15, 1, 0x00);
    delay1(1000);
		LCD_clear();
}

void signal_display()
{
	static bit a=0;
	setchar();
	
	if (isRCOffline == 0)
	{
		if (a)
		{
		LCD_clear();
		a = 0;
		}
		DisplayOneChar(0, 0, 0x01);
	}
	else 
	{
		if (a == 0)
		{
		LCD_clear();
		a = 1;
		}
		DisplayOneChar(0, 0, '!');
  }
}
//清屏
void LCD_clear()
{
    LCD_write_command(0x01);
}
