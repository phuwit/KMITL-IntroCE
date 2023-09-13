#include "ST7735_TEE.h"

TEE_ST7735 lcd(9, 10, 11, 12, 13); //Arduino  csk,sda,A0,rst,cs

void setup()
{
    //lcd.init(lcd.HORIZONTAL);
    lcd.init(lcd.VERTICAL);
}

void testRact(int mode)
{
    lcd.fillScreen(BLACK);
    for (int i = 0; i < 12; i++)
    {
        lcd.drawRect(i * 5, i * 5, lcd.width - i * 10, lcd.height - i * 10, WHITE);
        delay(250);
        if (mode == 1)
            lcd.drawRect(i * 5, i * 5, lcd.width - i * 10, lcd.height - i * 10, BLACK);
    }
}

void testCircle(int mode)
{
    lcd.fillScreen(BLACK);
    for (int i = 0; i < 12; i++)
    {
        lcd.drawCircle(lcd.width / 2, lcd.height / 2, (lcd.width / 2) - i * 5, WHITE);
        delay(250);
        if (mode == 1)
            lcd.drawCircle(lcd.width / 2, lcd.height / 2, (lcd.width / 2) - i * 5, BLACK);
    }
}

void testFillCircle()
{
    lcd.fillScreen(BLACK);
    for (int i = 0; i < 12; i++)
    {
        lcd.fillCircle(lcd.width / 2, lcd.height / 2, i * 5, YELLOW);
        delay(250);
    }
}

void testVHLine(char direction)
{
    lcd.fillScreen(BLACK);
    if (direction == 'V')
    {
        for (int i = 0; i < lcd.width; i += 5)
        {
            lcd.drawFastLine(i, 0, lcd.height - 1, YELLOW, 1); // 1 = Vertical, 0 = Herizental
        }
    }
    else
    {
        for (int i = 0; i < lcd.height; i += 5)
        {
            lcd.drawFastLine(0, i, lcd.width - 1, YELLOW, 0); // 1 = Vertical, 0 = Herizental
        }
    }
    delay(250);
}

void testAnyLine()
{
    lcd.fillScreen(BLUE);
    for (int x = 0; x < lcd.width; x += 6)
    {
        lcd.drawLine(0, 0, x, lcd.height - 1, YELLOW);
    }
    //lcd.fillScreen(BLUE);
    for (int x = lcd.width; x > 0; x -= 6)
    {
        lcd.drawLine(lcd.width-1, 0, x, lcd.height - 1, YELLOW);
    }
    for (int x = 0; x < lcd.width; x += 6)
    {
        lcd.drawLine(0, lcd.height-1, x, 0, YELLOW);
    }
    for (int x = lcd.width; x > 0; x -= 6)
    {
        lcd.drawLine(lcd.width-1, lcd.height - 1, x, 0, YELLOW);
    }    
}

void test_char()
{
    lcd.fillScreen(BLACK);
    lcd.drawChar(10, 20, 'H', YELLOW, 1);
    lcd.drawChar(20, 20, 'H', RED, 2);
    lcd.drawChar(35, 20, 'H', GREEN, 3);    
    lcd.drawChar(55, 20, 'H', CYAN, 4);       
    lcd.drawChar(80, 20, 'H', MAGENTA, 5);      
    lcd.drawString(10, 70, "Hello", YELLOW, 1);    
    lcd.drawString(10, 80, "Hello", RED, 2);    
    lcd.drawString(10, 95, "Hello", GREEN, 3);       
    lcd.drawString(10, 120, "Hello", CYAN, 4);  
    delay(1000);     

}

void loop()
{
    testRact(0);
    testRact(1);
    testCircle(0);
    testCircle(1);
    testFillCircle();
    testVHLine('V');
    testVHLine('H');
    testAnyLine();  
    test_char();
}
