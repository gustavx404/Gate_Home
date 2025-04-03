# Projeto Portão com ESP32 e MQTT

Este projeto utiliza um **ESP32** para controlar a abertura e o fechamento de um portão, com dois relés e um sensor Hall. A comunicação é feita via **MQTT**, permitindo o controle remoto do portão.

## Funcionalidade

- **Controle Remoto via MQTT**: O ESP32 se comunica com um servidor MQTT para abrir e fechar o portão.
- **Controle de Posição do Portão**: O sensor Hall detecta se o portão está aberto ou fechado.
- **Relés para Abertura e Fechamento**: Dois relés controlam os motores do portão.

## Componentes

- **ESP32**
- **2 Relés** (para abrir e fechar o portão)
- **Sensor Hall** (para detectar a posição do portão)
- **Broker MQTT** (Exemplo: Mosquitto)
- **Fonte de Alimentação** (para alimentar o ESP32 e os relés)

## Como Funciona

O ESP32 se conecta a uma rede Wi-Fi e se inscreve em tópicos MQTT para receber comandos de abertura e fechamento do portão. A posição do portão é monitorada pelo sensor Hall. Quando o sensor detecta que o portão está em uma posição específica (aberto ou fechado), o ESP32 publica essa informação em um tópico MQTT.

## Conexões

- **Relé 1 (Abertura)**: GPIO 23
- **Relé 2 (Fechamento)**: GPIO 22
- **Sensor Hall**: GPIO 21
- **Conexão Wi-Fi**: Defina suas credenciais de Wi-Fi no código

## Como Usar

### Passo 1: Configurar o Broker MQTT

1. Instale e configure um broker MQTT, como o [Mosquitto](https://mosquitto.org/).
2. Crie tópicos MQTT para controlar o portão:
   - **`portao/abrir`** para abrir o portão
   - **`portao/fechar`** para fechar o portão
   - **`portao/posicao`** para monitorar a posição do portão (aberto ou fechado)

### Passo 2: Clonar o Repositório

Clone o repositório para o seu computador:

```bash
git clone https://github.com/gustavx404/Portao_esp32.git
