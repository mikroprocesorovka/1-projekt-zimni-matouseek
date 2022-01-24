#include "stm8s.h"
#include "milis.h"
#include "stm8_hd44780.h"
#include "spse_stm8.h"



void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    lcd_init(); //inicializace LCD
    init_milis(); //inicializace mmilisu


}

void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned int value)
{
    char tmp[5] = {0x20, 0x20, 0x20, 0x20, '\0'} ;
    tmp[0] = (((value / 100) % 10) + 0x30);
    tmp[1] = (((value / 10) % 10) + 0x30);
    tmp[2] = 44;
    tmp[3] = ((value % 10) + 0x30);
    lcd_gotoxy(x_pos,y_pos);
    lcd_puts(tmp);
}

void ADC_init(void){
// na pinech/vstupech ADC_IN2 (PB2) a ADC_IN3 (PB3) vypneme vstupní buffer
ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL2,DISABLE);
// nastavíme clock pro ADC (16MHz / 4 = 4MHz)
ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
// volíme zarovnání výsledku (typicky vpravo, jen vyjmečně je výhodné vlevo)
ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
// nasatvíme multiplexer na některý ze vstupních kanálů
ADC2_Select_Channel(ADC2_CHANNEL_2);
// rozběhneme AD převodník
ADC2_Cmd(ENABLE);
// počkáme než se AD převodník rozběhne (~7us)
ADC2_Startup_Wait();
}

int main(void)
{
    uint32_t time = 0; //inicializace proměnných
    uint16_t adc_value;
    int16_t teplota;


    setup();
    ADC_init();


    while (1) {
     if (milis()-time>300){
        time = milis();
        
        adc_value = ADC_get(ADC2_CHANNEL_2); // do adc_value ulož výsledek převodu vstupu ADC_IN2 (PB2)
        teplota = (uint32_t)adc_value*3;

        lcd_print(0,0,teplota);
        lcd_gotoxy(4,0);
        lcd_puts(" C");
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
