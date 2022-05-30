#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "UnbufferedSerial.h"

//création de l'objet avec l'HC05
static UnbufferedSerial serial_read(PC_6,PC_7);

int main()
{

    //initialisation des codes LCD
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    
    int data = 0;
    int datapr = 0;
        
    //intro LCD avec plusieurs formes
    BSP_LCD_Clear(LCD_COLOR_GREEN);
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        BSP_LCD_DrawRect(10, 20, 50, 50);
        BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
        BSP_LCD_DrawCircle(80, 80, 50);
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
        BSP_LCD_DrawEllipse(150, 150, 50, 100);
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_FillCircle(200, 200, 40);
        HAL_Delay(2000);
        
    //Remise au propre LCD
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
        BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"Ul Mickael", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"Marie-Louise Killian", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"Reina Tibault", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"leprevier-Destas Nathan", CENTER_MODE);
        HAL_Delay(2000);
        BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)"Synthese Musicale", CENTER_MODE);
        HAL_Delay(2000);
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        
        
        /*Init transmission UART*/
        serial_read.baud(115200); //les bauds de la carte qui a été mesuré
        serial_read.format(8, SerialBase::None,1);//(nb de bit par trame, pas de parité, un seul stop bit)
        

    while (1) {
        datapr = data;
        data = 0;
            /* affichage "lecture du message" sur le LCD si un message peut être lu*/
        if(serial_read.readable()==true){
            BSP_LCD_Clear(LCD_COLOR_WHITE);
            BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"lecture du message", CENTER_MODE);
            serial_read.read(&data, 1);
        }
            
        switch (data){
            case 0:
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"attente d'une note", CENTER_MODE);
                break;
                
            case 0x01:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing do", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x02:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing re", CENTER_MODE);
                HAL_Delay(200);
                break;
                
            case 0x04:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing mi", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x08:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing fa", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x10:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing sol", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x20:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing la", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x40:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing si", CENTER_MODE);
                HAL_Delay(2000);
                break;
            
            case 0x80:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing do", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x81:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing do2", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x82:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing re2", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x84:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing fa2", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x88:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing sol2", CENTER_MODE);
                HAL_Delay(2000);
                break;
                
            case 0x90:
                BSP_LCD_Clear(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"playing la2", CENTER_MODE);
                HAL_Delay(2000);
                break;
            
            default :
                BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"note inconnu", CENTER_MODE);
                HAL_Delay(2000);
                break;
            }
                
                
            
        

    }
}
