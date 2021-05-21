#include <ESP8266WiFi.h>
#include <Servo.h>

Servo myServo;

//Call to ESpressif SDK
extern "C" {
  #include <user_interface.h>
}

//Hieronder kan je de SSID en Wachtwoord instellen.
//Op een Thomas More campus kan je TM_MoreWiFi gebruiken zonder wachtwoord

const char* ssid     = "WiFi-2.4-DF08";
const char* password = "CCfw21z1Ag5R";



int red_light_pin= 12;
int green_light_pin = 14;
int blue_light_pin = 15;



//^5c[-:]cf[-:]7f[-:].*
//Compileer problemen met onderstaande lijn? => 
//Verander YY YY YY in onderstaande lijn door cijfers uit uw studenten nummer !!!
uint8_t mac[6] {0x5C, 0xCF, 0x7F, 0x01, 0x09, 0x80};
//voorbeeld studenten nummer r123456 geeft: uint8_t mac[6] {0x5C, 0xCF, 0x7F, 0x12, 0x34, 0x56};

const char* host = "192.168.1.36";

void setup() {
  Serial.begin(115200);
 // Serial.println();
  
  //De volgende lijn veranderd het MAC adres van de ESP8266
  wifi_set_macaddr(0, const_cast<uint8*>(mac)); 
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Bezig met connecteren...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Geconnecteerd!\nMijn IP adres: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Mijn MAC adres is: %s\n", WiFi.macAddress().c_str());
  Serial.print("De gateway is: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("De DNS is: ");
  Serial.println(WiFi.dnsIP());

  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

   myServo.attach(4);
}

String btn_light;
 
int clicked = 0;

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}




void loop() {
  Serial.print("connecting to ");
  Serial.println(host);

  // Probeer te connecteren met de host 
  WiFiClient client;
  client.setTimeout(1000);    //Nodig voor de timeout in readStringUntil
  const int httpPort = 80;    //Een webserver luistert op poort 80
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // Het path klaar maken van hetgeen we willen van de host 
  String url = "/SmartTech2/esp8266/index.php?getStatus";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // Get naar de host sturen:
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(200);
  
  // Alles lezen en afprinten naar de seriele poort.
  // Merk op dat ook de antwoord headers worden afgedrukt!
  while(client.available()){
    btn_light = client.readStringUntil('\n');
    Serial.println(btn_light);

  }

              myServo.write(0); 


  if(btn_light.toInt() == 1){
            Serial.println("ochtendlicht");
            RGB_color(255, 255, 0); // Yellow
    
        
    }  
              
 
    if(btn_light.toInt() == 2){
            Serial.println("daglicht");
            RGB_color(0, 255, 255); // Cyan

  }
    if(btn_light.toInt() == 3){
            Serial.println("nachtlicht");  
            RGB_color(0, 0, 255); // Blue

  }


   if(btn_light.toInt() == 4){
    if( clicked == 0){
       Serial.println("voederen"); 
             // Make servo go to 0 degrees 
            myServo.write(180); 
            delay(500); 
           
    }
           
            
  }

     if(btn_light.toInt() == 5){
            Serial.println("reset"); 
             // Make servo go to 0 degrees 
            myServo.write(0); 
            delay(500); 
            
  }


  // De verbinding met de server sluiten 
  Serial.println();
  Serial.println("closing connection");
  client.stop();
  delay(100);
}
