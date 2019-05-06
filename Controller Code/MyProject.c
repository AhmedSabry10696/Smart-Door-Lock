sbit LCD_RS at RD2_bit;
sbit LCD_EN at RD3_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_RS_Direction at TRISD2_bit;
sbit LCD_EN_Direction at TRISD3_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;

#define Relay portd.b0
#define Buzzer portd.b1
#define on 1
#define off 0


char  keypadPort at PORTB;

int i;
char password[8];
char result;


char get_key()
{
 char k = 0;
 
 while (0 == k)
    k = Keypad_Key_Click();
    
 switch(k)
 {
  case 1: k = '1'; break;
  case 2: k = '2'; break;
  case 3: k = '3'; break;
  case 4: k = 'A'; break;
  case 5: k = '4'; break;
  case 6: k = '5'; break;
  case 7: k = '6'; break;
  case 8: k = 'B'; break;
  case 9: k = '7'; break;
  case 10: k = '8'; break;
  case 11: k = '9'; break;
  case 12: k = 'C'; break;
  case 13: k = '*'; break;
  case 14: k = '0'; break;
  case 15: k = '#'; break;
  case 16: k = 'D'; break;
 }
    
 return k;
}

char get_password()
{
    memset(password,0,8);
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"Enter Password :");
    for(i=0;i<4;i++)
    {
     password[i] = get_key();
     Lcd_out_chr(2, i+1,password[i]);
     delay_ms(100);
     }
     delay_ms(300);
}

void main()
{
    char key,error_count=0;
    trisd.b0 = 0;
    trisd.b1 = 0;


    Lcd_Init();
    keypad_Init();
    UART1_Init(9600);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    
    loop:
    Relay = 0;
    Buzzer = 0;
    get_password();
    UART1_Write_Text(password);
    UART1_Write(10);


    while(0 == UART1_Data_Ready()){}
    
    result = UART1_Read();
    if ('Y' == result)
    {
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1,1,"^_^ WELCOME ^_^");
     Relay = on;
     while(1)
     {
      key = get_key();
      if('*' == key)
      {
        Relay = ~Relay;
        uart1_write_text("changeState");
        UART1_Write(10);
      }
      else if('#' == key)
      {
        uart1_write_text("gotoStart");
        UART1_Write(10);
        error_count = 0;
        goto loop;
      }
     }
    }
    else if('N' == result)
    { 
      if(2 == error_count)
      {
       Lcd_Cmd(_LCD_CLEAR);
       Lcd_Out(1, 6, "Thief");
       Buzzer = on;
       uart1_write_text("Thief");
       UART1_Write(10);
       while(1)
       {
        if(uart1_data_ready())
          if('R' == uart1_read())
          {
             error_count = 0;
             goto loop;
          }
       }
      }
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(1, 3, "Error Password!!");
      Lcd_Out(2, 3, " Try Again! ");
      delay_ms(2000);
      error_count++;
      goto Loop;
    }

}