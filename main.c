#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUTTON_PIN 1
#define BUZZER_PIN 28
#define LED_PIN_RED 12
#define LED_PIN_GREEN 13
#define LED_PIN_YELLOW 11
#define LED_PIN_GREEN_PEDESTRIAN 22

// estado do botão. se for igual 1 irá iniciar semaforo em modo alternativo em alternate_btn()
int is_pressed = 0;

void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Desliga o PWM inicialmente
}

// Toca uma nota com a frequência e duração especificadas
void play_tone(uint pin, uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle

    sleep_ms(duration_ms);

    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
    sleep_ms(50); // Pausa entre notas
}

void play_up_sound(void){
  play_tone(BUZZER_PIN, 700, 100);
  play_tone(BUZZER_PIN, 730, 100);
  play_tone(BUZZER_PIN, 770, 100);
}

void play_down_sound(void){
  play_tone(BUZZER_PIN, 770, 100);
  play_tone(BUZZER_PIN, 730, 100);
  play_tone(BUZZER_PIN, 700, 100);
}

// checa se o botão foi pressionado, para iniciar o alternate_btn()
void button_pressed(void){
  if (gpio_get(BUTTON_PIN) == 0) { 
        is_pressed = 1;
        printf("Button pressed! \n");
  }
}

void setup_all_pins(void){
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_GREEN);
    gpio_init(LED_PIN_YELLOW);
    gpio_init(LED_PIN_GREEN_PEDESTRIAN);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_set_dir(LED_PIN_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_PIN_GREEN_PEDESTRIAN, GPIO_OUT);
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    pwm_init_buzzer(BUZZER_PIN);
}

// acende as luzes vermelha e a verde de pedestre juntas
void lights_up_rg(void){
  gpio_put(LED_PIN_RED, true);
  gpio_put(LED_PIN_GREEN_PEDESTRIAN, true);
}

void lights_down_rg(void){
  gpio_put(LED_PIN_RED, false);
  gpio_put(LED_PIN_GREEN_PEDESTRIAN, false);
}

// rotina normal de alternar semaforos. verde: 8s, amarelo: 2s, vermelho: 10s 
void alternate_normal(void){
  for(int i=0; i<=800; i++){
    gpio_put(LED_PIN_GREEN, true);
    // checa se o botão foi pressionado, para iniciar o alternate_btn()
    button_pressed();
    if (is_pressed) return;
    sleep_ms(10);
  }
  gpio_put(LED_PIN_GREEN, false);
  
  for(int i=0; i<=200;i++){
    gpio_put(LED_PIN_YELLOW, true);
    button_pressed();
    if (is_pressed) return;
    sleep_ms(10);
  }
  gpio_put(LED_PIN_YELLOW, false);
  
  for(int i=0; i<=1000; i++){
    gpio_put(LED_PIN_RED, true);
    button_pressed();
    if (is_pressed) return;
    sleep_ms(10);
  }
  gpio_put(LED_PIN_RED, false);
}

// rotina alternativa de semaforo. amarelo: 5s, vermelho: 15s. depois volta para normal
void alternate_btn(void){
  // reseta estado atual do semaforo
  gpio_put(LED_PIN_GREEN, false);
  gpio_put(LED_PIN_RED, false);
  gpio_put(LED_PIN_GREEN_PEDESTRIAN, false);

  for(int i=0; i<=500;i++){
    gpio_put(LED_PIN_YELLOW, true);
    button_pressed();
    sleep_ms(10);
  }
  gpio_put(LED_PIN_YELLOW, false);
  
  for(int i=0; i<=1500; i++){
    if (i % 400 == 0){
      // som intermitente
      play_up_sound();
    }
    lights_up_rg();
    button_pressed();
    sleep_ms(10);
  }
  // som para sinal de pedestre fechando
  play_down_sound();
  lights_down_rg();
  is_pressed = 0;
}

int main() {
    stdio_init_all();
    setup_all_pins(); 

    while (true) {
      if (is_pressed){
        alternate_btn();
        
      } else {
        alternate_normal();
      }        
    }
}