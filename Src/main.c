/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sam C
 * @brief          : Main program body
 ******************************************************************************
 */
#include "gpio.h"
#include "systick.h"
#include "nvic.h"
#include "uart.h"
#include "tim.h"
#include "room_control.h"

void heartbeat_led_toggle(void)
{
    static uint32_t last_tick = 0;
    if (systick_get_tick() - last_tick >= 500) { // Cambia cada 500 ms
        gpio_toggle_pin(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN);
        last_tick = systick_get_tick();
    }
}

/**
 * @brief Función principal del programa.
 *        Configura los periféricos y entra en un bucle infinito.
 *        El LED de heartbeat parpadea cada 500 ms.
*/
int main(void)
{
    // Inicialización de SysTick
    systick_init_1ms();
    
    // Configuración de LEDs
    gpio_setup_pin(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN, GPIO_MODE_OUTPUT, 0);
    gpio_setup_pin(GPIOA, EXTERNAL_LED_ONOFF_PIN, GPIO_MODE_OUTPUT, 0);
    
    // Configuración del botón
    gpio_setup_pin(GPIOC, 13, GPIO_MODE_INPUT, 0);
    nvic_exti_pc13_button_enable();
    
    // Configuración UART
    uart2_init(115200);
    nvic_usart2_irq_enable();
    
    // Configuración PWM
    tim3_ch1_pwm_init(1000); // Frecuencia de 1kHz
    tim3_ch1_pwm_set_duty_cycle(70); // Duty cycle inicial 70%
    
    // Inicialización de la aplicación
    room_control_app_init();
    
    // Mensaje inicial
    uart2_send_string("\r\nSistema de Control de Habitacion - Listo\r\n");
    uart2_send_string("Heartbeat LED funcionando\r\n");
    uart2_send_string("PWM inicializado al 70%\r\n");
    uart2_send_string("Esperando eventos...\r\n> ");
    
    // Bucle principal
    while (1) {
    heartbeat_led_toggle();
    room_control_on_systick(); // Cambiado de room_control_on_systick_tick()
    }
}