# Projeto de Semaforo para Raspberry Pi Pico

Projeto simples em C puro de Sistema Embarcado para semáforo com botoeira.

![semaforo](https://github.com/user-attachments/assets/e4f51240-0c72-4792-ac27-99adbe9b4b45)


## Objetivos

Desenvolver um sistema de controle de sinais de trânsito para um cruzamento, incluindo: 
- Sinal sonoro indicando permissão para pedestres atravessarem. 
- Botão de acionamento para deficientes visuais que desencadeia uma sequência de fechamento do sinal de trânsito para veículos, permitindo a travessia segura em todas no cruzamento da via. 

### Descrição do Projeto

Componentes Virtuais no Wokwi: 

- Raspberry Pi Pico para controle lógico.
- LEDs: Vermelho, amarelo e verde para representar o sinal de trânsito dos carros. 
- Um LED verde para pedestres. 
- Buzzer: para o som emitido quando o pedestre pode atravessar. 
- Push button: para simular a botoeira (acionador do sinal de pedestre). 
- Resistores: para os LEDs e botão. Protoboard e fiação. 
Linguagem:
 
- Todo o projeto deve ser desenvolvido a partir da linguagem C.
- O código deve estar estruturado (indentado) e as principais funções deverão estar comentadas. 
- Se possível, utilize funções e variáveis para simplificar o controle. 

## Funcionamento do Sistema 

### Estado Inicial: 
- Os LEDs para carros alternarão automaticamente (verde -> amarelo -> vermelho) seguindo uma lógica de tempo fixa (ex.: 8 segundos no verde, 2 no amarelo, 10 no vermelho). 
- O LED verde para pedestres estará desligado e o buzzer ficará inativo. 

Quando o botão é pressionado: 
- O led amarelo deve ficar acionado por 5 segundos e depois o vermelho deve ser acionado por 15 segundos, permitindo o tempo de fechamento seguro para os veículos.  
- A luz verde do pedestre deve ficar acesa pelo mesmo tempo em que a luz vermelha do semáforo estiver ativa (15 segundos). 
- Além disso, durante esse tempo, o buzzer deverá emitir um som intermitente, indicando que o sinal está aberto para o pedestre. 

Pós-travessia: 
- O sinal retorna ao funcionamento normal, alternando entre as luzes do semáforo do veículo. 
- O led verde do pedestre deve ser apagado e o buzzer deve ficar sem acionamento.

## Link para projeto no wokwi.com

[https://wokwi.com/projects/417561838196274177](https://wokwi.com/projects/417561838196274177)

![image](https://github.com/user-attachments/assets/50f30361-29a5-43a1-bdaa-bd2523cc3e87)
