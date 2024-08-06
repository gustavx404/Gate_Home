#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char *ssid = "**";
const char *password = "**";
const char *mqtt_server = "**";
const char *mqtt_user = "**";
const char *mqtt_pass = "**";
// --- Defini os GPIO do Hardware usado ---
#define control 16
#define endstop A0
#define sentido_a 12
#define sentido_b 13
#define led_a 4
#define led_b 5
// ---  Salva o modo de operação ---
int mode_n = 0;

// --- Vasriaveis que salvam o estado  ---
int endstopStatus = 1;
bool botaoPressionado = false;
bool botaoLiberado = true;
unsigned long tempoUltimoPressionamento = 0;
const unsigned long tempoDebounce = 250; // tempo de debounce em milissegundos
int UltimoValor = 0;
int TimeCounter = 0;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Portao", mqtt_user, mqtt_pass))
    {
      Serial.println("connected");
      client.publish("portao", "Conectado");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  pinMode(sentido_a, OUTPUT);
  pinMode(sentido_b, OUTPUT);
  pinMode(led_a, OUTPUT);
  pinMode(led_b, OUTPUT);
  pinMode(control, INPUT_PULLUP);

  digitalWrite(sentido_a, HIGH);
  digitalWrite(sentido_b, HIGH);
  digitalWrite(endstop, HIGH);
}
void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  delay(10);
  
  // Hall Sensor MQTT Discovery do Home Assistant
  String hall = "{\"expire_after\": \"600\", \"icon\": \"mdi:gate\", \"name\": \"Portao\", \"state_topic\": \"portao/state\"}";
  client.publish("homeassistant/sensor/portao/config",hall.c_str(),false);

  // Crie um botão no Home Assistant que envie uma mensagem MQTT para o ESP8266
  String button = "{\"device_class\": \"button\", \"expire_after\": \"600\", \"icon\": \"mdi:gate\", \"name\": \"Portao_Teste\", \"state_topic\": \"portao/button/state\", \"command_topic\": \"portao/button/command\"}";
  client.publish("homeassistant/button/portao/button/config", button.c_str(), false);

  int endstopAnalog = analogRead(endstop);
  // Serial.println(endstopAnalog);
  //  --- Verificação do Botão ---
  int estadoBotao = digitalRead(control);

  if (estadoBotao == LOW && botaoLiberado)
  {
    tempoUltimoPressionamento = millis();
    botaoPressionado = true;
    botaoLiberado = false;
    client.publish("portao/button/state","ON");
  }
  else if (estadoBotao == HIGH && !botaoLiberado && (millis() - tempoUltimoPressionamento) > tempoDebounce)
  {
    botaoLiberado = true;
    client.publish("portao/button/state","OFF");
  }

  if (botaoPressionado)
  {
    Serial.println("Portão Acionado");
    mode_n = (mode_n + 1) % 4;
    botaoPressionado = false;
  }
  // --- Sensor Hall Polo Norte do Ima ---
  else if (endstopAnalog <= 650 && mode_n == 3)
  {
    mode_n = 0;
  }
  // --- Sensor Hall Polo Sul do Ima ---
  else if (endstopAnalog >= 750 && mode_n == 1)
  {
    mode_n = 2;
  }
  else
  {
    botaoPressionado = false;
    return;
  }
  // --- Switch de controle do motor e lado ---
  switch (mode_n)
  {
  case 0:
    digitalWrite(sentido_a, HIGH);
    digitalWrite(sentido_b, HIGH);
    digitalWrite(led_a, LOW);
    digitalWrite(led_b, LOW);
    Serial.println("Parado");
    if (endstopAnalog <= 650){
      client.publish("portao/state","Fechado");
    }
    else {
      client.publish("portao/state","Parado");
    }
    break;
  case 1:
    digitalWrite(sentido_a, HIGH);
    digitalWrite(sentido_b, LOW);
    digitalWrite(led_a, HIGH);
    Serial.println("Abrindo");
    client.publish("portao/state","Abrindo");
    break;
  case 2:
    digitalWrite(sentido_a, HIGH);
    digitalWrite(sentido_b, HIGH);
    digitalWrite(led_a, LOW);
    digitalWrite(led_b, LOW);
    if (endstopAnalog >= 750){
      client.publish("portao/state","Aberto");
    }
    else {
      client.publish("portao/state","Parado");
    }
    break;
  case 3:
    digitalWrite(sentido_a, LOW);
    digitalWrite(sentido_b, HIGH);
    digitalWrite(led_b, HIGH);
    Serial.println("Fechando");
    client.publish("portao/state","Fechando");
    break;
  }
  delay(500);
}