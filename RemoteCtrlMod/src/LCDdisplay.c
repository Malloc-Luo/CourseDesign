#include <reg51.h>
#include "intrins.h"
#include "LCDdisplay.h"

/*
 * 注意！当按键有改变数字的时候或者发出重置命令的时候
 * 将这两个变量置1
 */
uint8_t isSetValChanged = 0;
uint8_t isResetRefVal = 0;

sbit SCL = P2^0;
sbit SDA = P2^1;


char ADDR = 0x4E;    // PCF8574  T  模块的地址码

unsigned char str_set[] = "Preset";
unsigned char str_actul[] = "Actual";
unsigned char degree_centi[]={0x16,0x09,0x08,0x08,0x08,0x09,0x06,0x00};//自定义字符

unsigned char *s1=str_set;
unsigned char *s2=str_actul;
unsigned char set_value=0;
unsigned char actul_value=0;

//***************************** 延时 y  ms ***********************************************

void delay1(int y)   // 
{
      
        while(y--)
        {
        unsigned char a,c;
        for(c=1;c>0;c--)
        {	
						for(a=2;a>0;a--);
				}
        }
}


//******************************** IIC 串口开始 ********************************************

void IIC_start(void)
{
        SDA=1;
        _nop_();
        SCL=1;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        SDA=0;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        SCL=0;
}


//********************************** IIC 串口写1个字节 ******************************************

void IIC_writeByte(char temp)
{
        char i;
        for(i=0;i<8;i++)
        {
                SDA=(bit)(temp & 0x80) ;   // 根据规定1602的数据最高位必须为  1  
                temp <<=1;
                _nop_();
                _nop_();
                SCL=1;
                _nop_();
                _nop_();
                _nop_();
                _nop_();
                _nop_();
                SCL=0;
        }
        _nop_(); 
        _nop_();
        _nop_();
        _nop_();
        SDA=1;
        _nop_(); 
        _nop_(); 
        _nop_();
        _nop_();
        SCL=1; 
        _nop_();
        _nop_();
        _nop_();
        while(SDA);
        _nop_();
        SCL=0;
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
        if(y<0)
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


//-------------------------------------------- 显示字符串的函数 ----------------------------------------------------

void LCD_write_word(unsigned char *s)                  //显示字符串的函数
{
        while(*s>0)
        {
                LCD_write_data(*s);
                s++;
        }
}




//*************按指定位置显示一个字符(针对1602液晶)-用在温度显示*********************

void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
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
  unsigned char i;
  LCD_write_command(0x40); // //设置第一个字的起始地址
  for(i=0;i<8;i++)
  {
    LCD_write_data(degree_centi[i]);
  } 
}


//*************************************************************************************************
//LCD显示 实际值、设定值
void LCD_display(unsigned int setval, unsigned int actulval)
{
		unsigned int setval_100,setval_10,setval_1;
		unsigned int actulval_100,actulval_10,actulval_1;
	
		setchar();
	
			//Write_LCD(0,0,str_set);
		Write_LCD(9,0,str_set);
		Write_LCD(0,0,str_actul);
		DisplayOneChar(5,1,0x00);
		DisplayOneChar(14,1,0x00);
	
		DisplayOneChar(2,1,'.');
		DisplayOneChar(11,1,'.');
		DisplayOneChar(7,0,'|');
		DisplayOneChar(7,1,'|');

		actulval_100 = actulval / 100;
		actulval_10 = (actulval - 100*actulval_100) / 10;
		actulval_1 = actulval - 100*actulval_100 -10*actulval_10;
	
		setval_100 = setval / 100;
		setval_10 = (setval - 100*setval_100) / 10;
		setval_1 = setval - 100*setval_100 -10*setval_10;
	
		DisplayOneChar(0,1,actulval_100 + '0');
		DisplayOneChar(1,1,actulval_10 + '0');
		DisplayOneChar(3,1,actulval_1 + '0');
	
		DisplayOneChar(9,1,setval_100 + '0');
		DisplayOneChar(10,1,setval_10 + '0');
		DisplayOneChar(12,1,setval_1 + '0');
	
}

//*************************************************************************************************

//遥控器按键设置

