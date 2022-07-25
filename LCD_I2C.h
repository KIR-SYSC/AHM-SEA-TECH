#ifndef _LCD_I2C_h
#define _LCD_I2C_h

#include "Arduino.h"
struct OutputState
{
    uint8_t rs = 0;
    uint8_t rw = 0;
    uint8_t E = 0;
    uint8_t Led = 0;
    uint8_t data = 0;

    uint8_t GetLowData()
    {
        uint8_t buffer = rs;
        buffer |= rw << 1;
        buffer |= E << 2;
        buffer |= Led << 3;
        buffer |= (data & 0x0F) << 4;

        return buffer;
    }

    uint8_t GetHighData()
    {
        uint8_t buffer = rs;
        buffer |= rw << 1;
        buffer |= E << 2;
        buffer |= Led << 3;
        buffer |= (data & 0xF0);
        return buffer;
    }
};

class LCD_I2C : public Print
{
public:
    LCD_I2C(uint8_t address, uint8_t columns = 16, uint8_t rows = 2)
    : _address(address), _columnMax(--columns), _rowMax(--rows) {}

    void begin(bool beginWire = true);
    void backlight();
    void noBacklight();

    void clear();
    void home();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();
    void display();
    void noDisplay();
    void cursor();
    void noCursor();
    void blink();
    void noBlink();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void createChar(uint8_t location, uint8_t charmap[]);
    void setCursor(uint8_t col, uint8_t row);

    // Method used by the Arduino class "Print" which is the one that provides the .print(string) method
    virtual size_t write(uint8_t character);

private:
    void InitializeLCD();
    void I2C_Write(uint8_t output);
    void LCD_Write(uint8_t output, bool initialization = false);

private:
    uint8_t _address;
    uint8_t _columnMax;
    uint8_t _rowMax;
    OutputState _output;
    uint8_t _displayState = 0x00;
    uint8_t _entryState = 0x00;
};

#endif
