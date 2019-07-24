#include<SoftwareSerial.h>
void esp8266();
#define RX 10
#define TX 11
// The Arduino pins 10 and 11 are configured to serial receiver and transmitter through software serial function. 

SoftwareSerial ser(RX,TX);
int val, sound;
unsigned long int channel=613784;
const char* apiKey="your_api_key";
//const char* server = "api.thingspeak.com";

//put your setup code here, to run once
void setup()
{
      //AT+CIOBAUD=9600 or AT+UART_DEF=9600,8,1,0,0 to change Baud rate of ESP module.
      
      Serial.begin(9600);
      ser.begin(9600);
           // The Wi-Fi module can be connected with the ThingSpeak server by sending AT commands from the module.
           // The module first test the AT startup by sending the command AT

      ser.println("AT");  // Attenuation
      delay(1000);
      // the AT command to set the connection to Wi-Fi mode is sent as follow - 
      ser.println("AT+CWMODE=3"); // To determine WiFi mode
      
     /* By setting the parameter in CWMODE to 3, the Wi-Fi connection is configured to SoftAP as well as station mode.
        This AT command can in fact take three parameters as follow -  
        1 - set Wi-Fi connection to station mode
        2 - set Wi-Fi connection to SoftAP mode
        3 - set Wi-Fi connection to SoftAP + station mode */

      delay(1000);
      ser.println("AT+RST"); // To restart the module
      delay(3000);

       // AT command to setup multiple connections is  enabled by sending the following command
     ser.println("AT+CIPMUX=1");
      // This AT command can take two parameters - 0 for setting single connection and 1 for setting multiple connections

      delay(1000);
      String cmd="AT+CWJAP=\"Name\",\"Password\""; // connect to Wi-Fi 
      ser.println(cmd);
      Serial.println(cmd);
      Serial.println("wifi connected");
      delay(1000);
      ser.println("AT+CIFSR"); // Return or get the local IP address
      delay(1000);
}


// put your main code here, to run repeatedly:
void loop()
{

      /* Now, the module is ready to establish TCP IP connection with the ThingSpeak server.
            The controller reads the sensor data and stores it in a string variable.
            The TCP IP connection is established by sending the following AT command  */
      
      AT + CIPSTART = 4, "TCP", "184.106.153.149", 80
            
       /* For establishing a TCP-IP connection, the command takes four parameters where first parameter is link ID which can be a number between 0 to 4,
       second parameter is connection type which can be TCP or UDP,
       third parameter is remote IP address or IP address of the cloud service to connect with and last parameter is port number. */

    String cmd = "AT+CIPSTART=4,\"TCP\",\"";
    cmd += "184.106.153.149"; // api.thingspeak.com
    cmd += "\",80";
    ser.println(cmd);
    Serial.println(cmd); 
    if(ser.find("Error"))
    {
        Serial.println("AT+CIPSTART error");
        return;
    }
    val=analogRead(A2);
    Serial.print("Air: ");
    Serial.print(val);
    sound=analogRead(A5);
    Serial.print("Sound: ");
    Serial.println(sound);
    
    String getStr = "GET /update?api_key=";
    getStr += apiKey;
    getStr +="&field1=";
    getStr +=String(val);
    getStr +="&field2=";
    getStr +=String(sound);
    getStr += "\r\n\r\n";

      /* Now when the connection with the server is successfully established and the controller has read the sensor value,
          it can send the data to the cloud using the following command AT+CIPSEND.
          And the second parameter is the data length which can be maximum 2048 bytes long. */

    cmd = "AT+CIPSEND=4,";
    cmd += String(getStr.length());
    ser.println(cmd);
    Serial.println(cmd);
    delay(1000);
    ser.println(getStr);
    Serial.println(getStr);
    delay(15000);  // Thingspeak needs minimum 15 sec delay between updates
}
