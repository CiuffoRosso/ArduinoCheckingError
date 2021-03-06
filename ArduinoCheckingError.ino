/*
ArduinoCheckingError

This sketch connects to a local logs website to check the system status.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Bicolor Led to pins 6, 7
 * Buzzer to pins 2, 4
  
 created 26 Jun 2017
*/

#include <SPI.h>
#include <Ethernet.h>

#define Led_pin0 6
#define Led_pin1 7

#define BuzzerGND 2
#define BuzzerVCC 4

#define BeepCorto 100
#define BeepLungo 200
#define SPENTO 0
#define ROSSO 1
#define VERDE 2

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char server[] = "dcapplc.asl19.ad";

IPAddress ip(192, 168, 0, 177);

EthernetClient client;

boolean newLivAllertaIncoming = false;
byte prec;
byte liv_allerta;

unsigned long lastBuzzerTime = 0;
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 30000L;
const unsigned long buzzerInterval = 180000L;

void setup() {
  Serial.begin(9600);

  pinMode(Led_pin0, OUTPUT);
  pinMode(Led_pin1, OUTPUT);

  pinMode(BuzzerGND, OUTPUT);
  pinMode(BuzzerVCC, OUTPUT);
  digitalWrite(BuzzerGND, 0);

  Serial.begin(9600);
  while (!Serial) {  }
  
  Serial.println("Ethernet Initializing...");
  delay(1000); // give the ethernet module time to boot up
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Serial.println("Initializing using a fixed IP");
    Ethernet.begin(mac, ip);   
  }
  Serial.println("Ethernet Initialized.");

  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  if (client.available())
  {
    liv_allerta = client.read();
    if (newLivAllertaIncoming)
    {
      liv_allerta -= 48;
      Serial.print("Livello di allerta: ");
      Serial.println(liv_allerta);
      AccendiLed();
      newLivAllertaIncoming = false;
      prec = liv_allerta;
    }
    if (liv_allerta == '#')
    {
      newLivAllertaIncoming = true;
    }
  }

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

}

// this method makes a HTTP connection to the server:
void httpRequest() {
  client.stop();
  if (client.connect(server, 8200))
  {
    Serial.println("\nConnected");
    // HTTP GET request:
    client.println("GET /cl HTTP/1.1");
    client.println("Host: dcapplc.asl19.ad:8200");
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();
  }
  else
  {
    //non è possibile connettersi alla rete
    Serial.println("\nNon è possibile connettersi alla rete!");
    SuonaBuzzer(1, BeepCorto);
    for (int i = 0; i < postingInterval / 2000; i++)
    {
      LedBicolor_ImpostaStato(ROSSO);
      delay(1000);
      LedBicolor_ImpostaStato(SPENTO);
      delay(1000);
    }
  }
}
