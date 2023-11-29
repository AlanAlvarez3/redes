#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>

 
// Update these with values suitable for your network.

const char* ssid = "alan";
const char* password = "12345678";
const char* mqtt_server = "3.87.135.5";

 
WiFiClient espClient;
PubSubClient client(espClient);



long lastMsg = 0;
char msg[50];
int value = 0;
IRrecv irrecv(5);
decode_results results;



void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(A0, INPUT);
  irrecv.enableIRIn();
  

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 8080); //1883
  client.setCallback(callback);
  // Comenzamos el sensor DHT
 
}
 
void setup_wifi() {
 
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
  
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("alanear")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      
      // ... and resubscribe
      client.subscribe("PrimoBajeseYaDelTsurito");
      //client.subscribe("Reci20");

      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void loop() {
 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  
  delay(5000);

  if(irrecv.decode(&results)){
    Serial.println(results.value, HEX);
    delay(1000);
    float pot = analogRead(A0);
    irrecv.resume();
    snprintf(msg, 75, "{\"Sensor\":%2.1f,\"Control\":\"%X\"}", pot, results.value);
    client.publish("SensorP1",msg);
  }
  
  //}

}
