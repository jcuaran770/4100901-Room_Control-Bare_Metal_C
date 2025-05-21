/**
 ******************************************************************************
 * @file           : room_control.h
 * @author         : Sam C
 * @brief          : Header file for Room Control Application
 ******************************************************************************
 */
#ifndef ROOM_CONTROL_H
#define ROOM_CONTROL_H

#define HEARTBEAT_LED_PORT     GPIOA
#define HEARTBEAT_LED_PIN      5       // LD2 en la placa NUCLEO-L476RG
#define EXTERNAL_LED_ONOFF_PIN 7       // PA7 para LED ON/OFF
#define EXTERNAL_LED_PWM_PIN   6       // PA6 para LED PWM (TIM3_CH1)

#include <stdint.h>

void room_control_on_systick_tick(void);
/**
 * @brief Función a ser llamada por EXTI15_10_IRQHandler cuando se detecta
 *        la pulsación del botón B1.
 * @note  Esta función debe ser llamada desde el manejador de interrupción
 *        EXTI15_10_IRQHandler.
 */

/**
 * @brief Función a ser llamada por EXTI15_10_IRQHandler cuando se detecta
 *        la pulsación del botón B1.
 */
void room_control_on_button_press(void);

/**
 * @brief Función a ser llamada por USART2_IRQHandler cuando se recibe un carácter.
 * @param received_char El carácter recibido por UART.
 */
void room_control_on_uart_receive(char received_char);

/**
 * @brief (Opcional) Función para realizar inicializaciones específicas de la lógica
 *        de room_control, si las hubiera (ej. inicializar variables de estado).
 *        Las inicializaciones de periféricos se harán en main().
 */
void room_control_app_init(void);

#endif // ROOM_CONTROL_H
