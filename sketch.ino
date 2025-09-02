/*
------------------ FIAP --------------------
CP4 Edge - Hands-On de Vinherias pelo Cloud IoT
EDGE COMPUTING & COMPUTER SYSTEMS
Participantes:
Prof. Paulo Marcotti PF2150
Daniel Laureano da Luz RM565899
Gabriel Del Sole Speck RM562513
Marcos Antonio Da Silva RM566451
Paulo Gustavo Pessoa da Silva RM566194
Rone Cruz Santos RM566558
--------------------------------------------
*/

// ===== Bibliotecas =====
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ===== Sensores =====
#define DHTPIN 19       // Pino do DHT
#define DHTTYPE DHT22   // Tipo de sensor DHT
DHT dht(DHTPIN, DHTTYPE);

const int LDR = 36; // Pino analógico do LDR
int valorsensor;

// ===== LED Onboard =====
const int LED_PIN = 2; 
char EstadoSaida = '0';

// ===== Configurações Wi-Fi & MQTT =====
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";               
const char* BROKER_MQTT = "20.15.104.27"; // Conexão com o MyMqtt
const int BROKER_PORT = 1883;
const char* TOPICO_SUBSCRIBE = "/TEF/lamp001/cmd";
const char* TOPICO_PUBLISH_1 = "/TEF/lamp001/attrs";       // Estado do LED
const char* TOPICO_PUBLISH_TEMP = "/TEF/lamp001/attrs/t";  // Temperatura
const char* TOPICO_PUBLISH_UMID = "/TEF/lamp001/attrs/h";  // Umidade
const char* TOPICO_PUBLISH_LUMI = "/TEF/lamp001/attrs/l";  // Luminosidade
const char* ID_MQTT = "fiware_vinheria_001";

// Intervalo entre os envios
unsigned long ultimoEnvio = 0; 
const long intervalo = 15000; 

WiFiClient espClient;
PubSubClient MQTT(espClient);

// ===== Inicialização =====
void initSerial() {
  Serial.begin(115200);
}

void initWiFi() {
  delay(10);
  Serial.println("------ Conexao Wi-Fi ------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  // Iniciando conexão na rede
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado com sucesso ao Wi-Fi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Iniciando conexão com o MyMQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

// ===== Setup =====
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  initSerial();
  dht.begin();
  initWiFi();
  initMQTT();
}

// ===== Loop =====
void loop() {
  VerificaConexoesWiFIEMQTT();
  MQTT.loop();

  unsigned long agora = millis();

  // Caso tenha passado o intervalo:
  if (agora - ultimoEnvio >= intervalo) {
    ultimoEnvio = agora;

    // Leitura dos sensores
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    valorsensor = analogRead(LDR);

    if (isnan(h) || isnan(t)) {
      Serial.println("Falha ao ler o sensor DHT!");
      return;
    }

    // Publica os dados no broker
    MQTT.publish(TOPICO_PUBLISH_TEMP, String(t).c_str());
    MQTT.publish(TOPICO_PUBLISH_UMID, String(h).c_str());
    MQTT.publish(TOPICO_PUBLISH_LUMI, String(valorsensor).c_str());

    Serial.println("===== Dados enviados =====");
    Serial.print("Temperatura: "); Serial.println(t);
    Serial.print("Umidade: "); Serial.println(h);
    Serial.print("Luminosidade: "); Serial.println(valorsensor);

    // Envia estado do LED
    EnviaEstadoOutputMQTT();
  }
}

// ===== Callbacks e Conexões =====
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.print("Mensagem recebida: ");
  Serial.println(msg);

  // Recebe as mensagens do app e traduz
  if (msg.equals("lamp001@on|")) {
    digitalWrite(LED_PIN, HIGH);
    EstadoSaida = '1';
  }
  if (msg.equals("lamp001@off|")) {
    digitalWrite(LED_PIN, LOW);
    EstadoSaida = '0';
  }
}

void VerificaConexoesWiFIEMQTT() {
  if (WiFi.status() != WL_CONNECTED) {
    initWiFi();
  }
  if (!MQTT.connected()) {
    reconnectMQTT();
  }
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar ao Broker MQTT...");
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado!");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    } else {
      Serial.print("Falha. rc=");
      Serial.print(MQTT.state());
      Serial.println(" Tentando novamente em 2s");
      delay(2000);
    }
  }
}

void EnviaEstadoOutputMQTT() {
  if (EstadoSaida == '1') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
    Serial.println("LED Ligado");
  } else {
    MQTT.publish(TOPICO_PUBLISH_1, "s|off");
    Serial.println("LED Desligado");
  }
}