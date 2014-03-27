#define SET_TRIS    0x01
#define SET_LAT     0x02
#define GET_PORT    0x03
#define GET_BUT     0x05
#define GET_ID      0x20
#define GET_FRM     0x21
#define LCD_CLR     0x60
#define LCD_WR      0x61
#define SET_BL      0x62
#define UART_EN     0x10

#define BOARD_ID    0x65
#define ADDRESS     0x30


/**
 * @author      Stefan Mavrodiev @ OLIMEX LTD <support@olimex.com>
 * @version     1.0
 * @since       2013-11-03
 */
class LCD16x2{
    public:
        
        LCD16x2();
        ~LCD16x2();
        
        uint8_t getID();
        uint8_t getFirmwareVersion();
        
        void uartEnable(bool state);
        
        void pinMode(uint8_t pin, uint8_t direction);
        void digitalWrite(uint8_t pin, uint8_t level);
        uint8_t digitalRead(uint8_t pin);
        uint8_t readButtons();    
        
        void lcdSetBlacklight(uint8_t value);
        void lcdClear();
        void lcdGoToXY(uint8_t x, uint8_t y);
        void lcdWrite(char *string);    
        
        const static uint8_t GPIO1 = 1;
        const static uint8_t GPIO2 = 2;
        const static uint8_t GPIO3 = 3;
        const static uint8_t GPIO4 = 4;
        const static uint8_t GPIO5 = 5;
        const static uint8_t GPIO6 = 6;
        const static uint8_t GPIO7 = 7;
        const static uint8_t GPIO8 = 8;
        const static uint8_t GPIO9 = 9;        
        
    private:
        uint8_t X;
        uint8_t Y;
};
