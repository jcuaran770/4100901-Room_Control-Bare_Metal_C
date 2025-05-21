/**
 ******************************************************************************
 * @file           : room_control.c
 * @author         : Sam C
 * @brief          : Room control driver for STM32L476RGTx
 ******************************************************************************
 */
#include "room_control.h"

#include "gpio.h"    // Para controlar LEDs y leer el botón (aunque el botón es por EXTI)
#include "systick.h" // Para obtener ticks y manejar retardos/tiempos
#include "uart.h"    // Para enviar mensajes
#include "tim.h"     // Para controlar el PWM


// Variables de estado (static para mantener privacidad)
static volatile uint32_t led_onoff_start_time = 0;
static volatile uint8_t led_onoff_active = 0;
static volatile uint32_t last_button_press_time = 0;


void room_control_app_init(void)
{
    // Inicializar variables de estado si es necesario.
     /* Poner LEDs apagados */
    // Por ejemplo, asegurar que los LEDs estén apagados al inicio
    // Inicializar LEDs
    gpio_write_pin(GPIOA, EXTERNAL_LED_ONOFF_PIN, GPIO_PIN_RESET); // LED ON/OFF apagado
    tim3_ch1_pwm_set_duty_cycle(70); // PWM al 70% inicialmente
    
    // Mensaje de inicialización
    uart2_send_string("Sistema de control de habitacion inicializado\r\n");
    uart2_send_string("Comandos disponibles:\r\n");
    uart2_send_string("  h/H - PWM al 100%\r\n");
    uart2_send_string("  l/L - PWM al 0%\r\n");
    uart2_send_string("  t/T - Toggle LED ON/OFF\r\n");
    // tim3_ch1_pwm_set_duty_cycle(50); // Establecer un duty cycle inicial para el PWM LED
}

void room_control_on_button_press(void)
{
    // TODO: Implementar anti-rebote
    uint32_t now = systick_get_tick();

    /* Anti-rebote */
    // Anti-rebote: ignorar pulsaciones muy cercanas (200ms)
    if ((now - last_button_press_time) < 200) {
        return;
    }

    last_button_press_time = now;

    // Encender LED ON/OFF
    if (!led_onoff_active) {
        gpio_write_pin(GPIOA, EXTERNAL_LED_ONOFF_PIN, GPIO_PIN_SET);
        led_onoff_start_time = now;
        led_onoff_active = 1;
        uart2_send_string("Boton presionado - LED encendido por 3 segundos\r\n");
    }
}

void room_control_on_uart_receive(char received_char)
{
    /* Ajuste de PWM según comando */
    switch (received_char) {
        case 'H': case 'h':
            tim3_ch1_pwm_set_duty_cycle(100); // 100% duty cycle
            uart2_send_string("PWM al 100%\r\n");
            break;
        case 'L': case 'l':
            tim3_ch1_pwm_set_duty_cycle(0); // 0% duty cycle
            uart2_send_string("PWM apagado\r\n");
            break;
        case 'T': case 't':
            gpio_toggle_pin(GPIOA, 7); // Toggle LED ON/OFF
            uart2_send_string("Toggle LED ON/OFF\r\n");
            break;
        case '\r': case '\n':  // Ignorar retornos de carro
            break;
        default:
            uart2_send_string("Comando inválido: '");
            uart2_send_char(received_char);
            uart2_send_string("'. Use H/h, L/l o T/t\r\n");
            break;
    }
}

void room_control_on_systick(void)
{

    /* Apaga LED ON/OFF tras 3 s */
    if (led_onoff_active && (systick_get_tick() - led_onoff_start_time >= 3000)) {
        /* Apagar LED ON/OFF */
        gpio_write_pin(GPIOA, EXTERNAL_LED_ONOFF_PIN, GPIO_PIN_RESET);
        led_onoff_active = 0;
        uart2_send_string("LED ON/OFF apagado por timeout\r\n");
    }
}
