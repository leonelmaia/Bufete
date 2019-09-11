/*
 * led_panel.c
 *
 *  Created on: 12 de ago de 2019
 *      Author: Leonel Maia
 */

#include "led_panel.h"



/**
  * @brief  Inicializacao do painel de LEDs
  * @param  led_panel: Handle do painel de leds
  * @retval void
  */
void led_panel_init(led_panel_t *led_panel){

  for(uint8_t number_leds = 0; number_leds < led_panel->num_of_leds; number_leds++){
    /*Todos os LEDs sao inicialmente desligados*/
    led_panel->leds[number_leds].gpio_port->BRR = led_panel->leds[number_leds].gpio_pin;
  }
  return;
}

/**
  * @brief  Liga ou Desliga os LEDs do painel
  * @note   Esta funcao deve ser chamamda antes de qualquer uso do painel.
  * @param  led_panel: Handle do painel de leds
  * @param  led_panel_value: indica, atraves dos seus bits, que estado os LEDs devem assumir.
  * @verbatim
           Exemplo:
           O seguinte mapeamento se aplica a este parametro
           LED_1 | LED_2 | LED_3 | LED_4 | LED_5 | LED_6 | LED_7 | LED_8 | LED_9 | LED_10
           bit 9   bit 8   bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0

           led_panel_value = 0x000 -> desliga todos os LEDs
           led_panel_value = 0x3ff -> liga todos os LEDs
           led_panel_value = 0x005 -> liga os LEDs 9 e 7 e desliga os demais.
    @endverbatim
  * @retval void
  */
uint32_t led_panel_write(led_panel_t *led_panel, uint16_t led_panel_value){

	uint32_t msb = 0;

  for(uint8_t index_led = 0; index_led < led_panel->num_of_leds; index_led++){
    if( (led_panel_value >> index_led) & 0x01){
      led_panel->leds[index_led].gpio_port->BSRR =  led_panel->leds[index_led].gpio_pin;
      msb = index_led;
    }else{
    	led_panel->leds[index_led].gpio_port->BRR = led_panel->leds[index_led].gpio_pin;
    }
  }

  return msb;
}
