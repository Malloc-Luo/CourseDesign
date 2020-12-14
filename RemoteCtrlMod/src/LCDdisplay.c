#include "LCDdisplay.h"

/*
 * ע�⣡�������иı����ֵ�ʱ����߷������������ʱ��
 * ��������������1
 */
uint8_t isSetValChanged = 0;
uint8_t isResetRefVal = 0;

sbit SCL = P2^0;
sbit SDA = P2^1;

char ADDR = 0x4E;    // PCF8574  T  ģ��ĵ�ַ��
unsigned char str_set[] = "Preset";
unsigned char str_actul[] = "Actual";
unsigned char degree_centi[]={0x16,0x09,0x08,0x08,0x08,0x09,0x06,0x00};//�Զ����ַ����϶�
unsigned char *s1=str_set;
unsigned char *s2=str_actul;

unsigned char setval=0;
unsigned char actulval=0;

char i,set,actul;

//***************************** ��ʱ y  ms ***********************************************

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


//******************************** IIC ���ڿ�ʼ ********************************************

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


//********************************** IIC ����д1���ֽ� ******************************************

void IIC_writeByte(char temp)
{
        char i;
        for(i=0;i<8;i++)
        {
                SDA=(bit)(temp & 0x80) ;   // ���ݹ涨1602���������λ����Ϊ  1  
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


//******************************** 1602д���� ********************************************

void LCD_write_command(char comm)
{
        char tmp;
        IIC_start();          // ���ڿ�ʼ
        IIC_writeByte(ADDR);  // ��ѡPCF 8574T �ĵ�ַ  ��Ӧ�����൱��ѡ�е���˼�ɣ�
        
        tmp = comm & 0xF0;    // ��0xf0 Ӧ����ȡ����λ����˼��
        tmp |= 0x0C;         //������4λΪָ��ĸ���λ������λΪ   RS = 0, RW = 0, EN = 1  
        IIC_writeByte(tmp);  //�Ӵ����ͳ�
        delay1(5);
        tmp &= 0xFB;        //Make EN = 0
        IIC_writeByte(tmp); 
        
        tmp = (comm & 0x0F) << 4 ;  //��ָ��ĵ���λ �͵���λ�ñ���
        tmp |= 0x0C;        //RS = 0, RW = 0, EN = 1
        IIC_writeByte(tmp);
        delay1(5);
        tmp &= 0xFB; // Make EN = 0
        IIC_writeByte(tmp);
        
}
//******************************** 1602д���� ********************************************


void LCD_write_data(char data1)
{
        char tmp;
        IIC_start();
        IIC_writeByte(ADDR);   // ��ѡPCF 8574T �ĵ�ַ  ��Ӧ�����൱��ѡ�е���˼�ɣ�
        
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


//******************************** 1602��ʼ�� ********************************************


void Init_Lcd(void)
{
        LCD_write_command(0x33); //��8λ����תΪ4λ����
        delay1(5) ;
        LCD_write_command(0x32); //
        delay1(5) ; 
        LCD_write_command(0x28); // 4λ�����ߣ���ʾ2�У�5*7�����ַ�  �������0x38  ��Ϊ8λ�����ߣ���ʾ2�У�5*7�����ַ�
        delay1(5) ; 
        LCD_write_command(0x0C); // ����ʾ���رչ�꣬����˸
        delay1(5) ;  
        LCD_write_command(0x06); // �趨���뷽ʽ��������λ��
        delay1(5) ; 
        LCD_write_command(0x01); // ����
        delay1(5) ;
}


//*************************************** ��ָ��λ����ʾ�ַ��� *************************************

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
        
        addr = 0x80 + 0x40 * y + x;   // Move cursor  �ƶ����
        LCD_write_command(addr);
        while (*str) 
        {
                LCD_write_data(*str++); 
        }
}


//-------------------------------------------- ��ʾ�ַ����ĺ��� ----------------------------------------------------


void LCD_write_word(unsigned char *s)                  //��ʾ�ַ����ĺ���
{
        while(*s>0)
        {
                LCD_write_data(*s);
                s++;
        }
}



//��ָ��λ����ʾһ���ַ�(���1602Һ��)-�����¶���ʾ

void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	Y &= 0x1;
	X &= 0xF;                 //����X���ܴ���15��Y���ܴ���1
	if (Y) X |= 0x40;        //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
	X |= 0x80;               // ���ָ����
	LCD_write_command(X);    //���ﲻ���æ�źţ����͵�ַ��
	LCD_write_data(DData);
}

//���趨�ִ���CGRAM

void setchar()  
{
  unsigned char i;
  LCD_write_command(0x40); //���õ�һ���ֵ���ʼ��ַ
  for(i=0;i<8;i++)
  {
    LCD_write_data(degree_centi[i]);
  } 
}
//*************************************************************************************************
//��ʾ��ǰ�趨ֵ��ʵ��ֵ

void LCD_display(unsigned int setval, unsigned int actulval)
{
		unsigned int setval_100,setval_10,setval_1;
		unsigned int actulval_100,actulval_10,actulval_1;
	
		setchar();
	
		Write_LCD(9,0,str_set);
		Write_LCD(0,0,str_actul);
		DisplayOneChar(5,1,0x00);
		DisplayOneChar(14,1,0x00);
	
		DisplayOneChar(2,1,'.');
		DisplayOneChar(11,1,'.');
		DisplayOneChar(7,0,'|');
		DisplayOneChar(7,1,'|');

	
		setval_100 = setval / 100;
		setval_10 = (setval - 100*setval_100) / 10;
		setval_1 = setval - 100*setval_100 -10*setval_10;
	
		actulval_100 = actulval / 100;
		actulval_10 = (actulval - 100*actulval_100) / 10;
		actulval_1 = actulval - 100*actulval_100 -10*actulval_10;
	
		DisplayOneChar(9,1,setval_100 + '0');
		DisplayOneChar(10,1,setval_10 + '0');
		DisplayOneChar(12,1,setval_1 + '0');
	
		
		DisplayOneChar(0,1,actulval_100 + '0');
		DisplayOneChar(1,1,actulval_10 + '0');
		DisplayOneChar(3,1,actulval_1 + '0');
}

//*************************************************************************************************

//ң������������

