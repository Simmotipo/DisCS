#include "arduino_secrets.h"
//Based on web server API by Tom Igoe on created 25 Nov 2012

//Required Libraries and Files
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "wrapper.h"
#include "discord.h"
//Contains Passwords etc. 

//Fetch Secret Data

int status = WL_IDLE_STATUS;     // the WiFi radio's status
unsigned long lastMillis = 0; //number of milliseconds into execution last command was processed
IPAddress ip;

WiFiServer server(80);

void setup() {
  //Set default pin settings
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, OUTPUT);
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    wifiStat[0] = 3;
    // don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
    wifiStat[0] = 3;
  }
}

//Connect to WiFi
void connect() {
  digitalWrite(13, LOW);
  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  // Connect to WPA/WPA2 network:
  status = WiFi.begin(ssid, pass);
  wifiStat[0] = 1;

  // wait 5 seconds for connectionn to ensure connection:
  delay(5000);
  if (status != WL_CONNECTED) { wifiStat[0] = 3; } else { printWiFiData(); wifiStat[0] = 2; }

  // you're connected now, so attempt automatic registration with discord bot:
  if (wifiStat[0] == 2) {
    digitalWrite(13, HIGH);
    Serial.println("/disc register " + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
    discord_send("/disc register " + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
    wifiStat[0] = 4;
    server.begin();                           // start the web server on port 80
    printServerStatus();                      // Print out the status
  }
  Serial.println("Connection Attempt Complete");
}

//Process WiFi Connectivity Status
void processStatus() {
  if (wifiStat[0] == 0) { connect(); } //If never connect, attempt connection
  if (wifiStat[0] == 3) { //If disconnected, flash LED
    while (wifiStat[0] == 3) {
      delay(500);
      if (wifiStat[1] == 0) { wifiStat[1] = 1; digitalWrite(13, HIGH); } else { wifiStat[1] = 0; digitalWrite(13, LOW); }
      if (digitalRead(11) == HIGH) { wifiStat[0] = 0; while (digitalRead(11) == HIGH) { delay(10); }} //Reconnect to WiFi
    }
  }
  if (digitalRead(11) == HIGH) { wifiStat[0] = 0; while (digitalRead(11) == HIGH) { delay(10); }} //Reconnect to WiFi
}

//Print Server Connectivity Status
void printServerStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void printWiFiData() {
  // print your board's IP address:
  ip = WiFi.localIP();
  Serial.print("IP address : ");
  Serial.println(ip);

  Serial.print("Subnet mask: ");
  Serial.println((IPAddress)WiFi.subnetMask());

  Serial.print("Gateway IP : ");
  Serial.println((IPAddress)WiFi.gatewayIP());

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  //Serial.print("MAC address: ");
  //printMacAddress(mac);
}


void loop() {
  processStatus();
  WiFiClient client = server.available();     // listen for incoming clients
    if (client) {                             // if you get a client,
      Serial.println("new client");           // print a message out the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character
  
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              sendHTTP(client);
              // break out of the while loop:
              break;
            } else {    // if you got a newline, parse then clear currentLine:
              parseHTTP(currentLine);
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
        
      }
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
    }
}


////WARNING: "FUN" STUFF BELOW


void parseHTTP(String currentLine) {
  String args[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
  //^Way longer than it will realistically ever need to be but better safe than sorry
  // Check to see if the client request was a command
  if (currentLine.endsWith(";;") and millis() > lastMillis + 25) {
    //We know the request came from Discord.
    int m = 0;
    int n = 0;
    while (m < currentLine.length()) {
      if (String(currentLine[m]) == " " || String(currentLine[m]) == ";") { n = n + 1; } else { args[n] = args[n] + currentLine[m]; }
      m = m + 1;
    }
    Serial.println("CMD >> " + currentLine);
    processCMD(args);
    lastMillis = millis();
  }
  else if (currentLine.startsWith ("GET") and currentLine.endsWith("/run") and millis() > lastMillis + 25) {
    //We know it's time to prepare for something to go wrong. Oh, and that the request came from a browser, of course. [Just kidding I fixed all the bugs. Yes, I still have flashbacks].
    //Mostly deprecated. Only used for setting pins as input/output and output pins as high/low
    int m = 0;
    int n = 0;
    while (m < currentLine.length()) {
      if (String(currentLine[m]) == "/" || String(currentLine[m]) == ";") { n = n + 1; } else { args[n] = args[n] + currentLine[m]; }
      //^ If current char indicates end of arg, move to the next arg, else append the char to the arg (ie process the arg char by char)
      m = m + 1;
      lastMillis = millis();
    }
    Serial.println("WEB >> " + currentLine); //Print line to console for debugging
    n = 3; //Start at 3 so all the HTTP pre-amble
    while (n < 24) { //"shift" all args three places to the left, so the first arg is at arg[0] not arg[3]
      args[n-3] = args[n];
      n = n + 1;
    }
    args[20] = ""; args[21] = ""; args[23] = ""; args[24] = "";
    processCMD(args); //Actually do the stuff now
  }
  else if (currentLine.startsWith("GET /HTP?") and millis() > lastMillis + 25) { //The data source is browser form
    Serial.println("HTP >> " + currentLine); //Print line to console for debugging
    //From here, take the string and look for all the end-of-arg characters (x) to process the args char by char to be able to reconstruct a string array (args[])
    String cmdConstruct = "";
    int z = 9;
    while (String(currentLine[z]) != String("x") and z != currentLine.length()) {
      cmdConstruct = cmdConstruct + currentLine[z];
      z = z + 1;
    }
    Serial.println(cmdConstruct);
    args[0] = cmdConstruct;
    int q = 1;
    z = z + 2;
    while (z - 2 != currentLine.length()) {
      cmdConstruct = "";
      while (String(currentLine[z]) != String("&") and z != currentLine.length() and String(currentLine[z]) != String(" ")) {
        cmdConstruct = cmdConstruct + currentLine[z];
        z = z + 1;
      }
      Serial.println("HTA >> " + cmdConstruct);
      args[q] = cmdConstruct;
      q = q + 1;
      while (z != currentLine.length() and String(currentLine[z]) != String("x")) { z++; }
      z = z + 2;
    }
    processCMD(args); //And do the actual stuff now
  }
}

void sendHTTP(WiFiClient client) {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  // the content of the HTTP response follows the header:
  
  //The HTML section which still gives me nightmares...
  //Send HTML code line-by line to construct browser page

  //Pin Input/Output and High/Low setting
  int q = 0;
  client.print("<p></p><form action=\"/HTP\"> <label for=\"digitalWritex\">Pin #</label> <input type=\"number\" id=\"digitalWritex\" name=\"digitalWritex\" value=\"0\">  <label for=\"x\">State</label> <select id=\"x\" name=\"x\"><option value=\"HIGH\">HIGH</option><option value=\"LOW\">LOW</option></select> <input type=\"submit\" value=\"digitalWrite\"> </form>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"pinModex\">Pin #</label> <input type=\"number\" id=\"pinModex\" name=\"pinModex\" value=\"0\">  <label for=\"x\">State</label> <select id=\"x\" name=\"x\"><option value=\"INPUT\">INPUT</option><option value=\"OUTPUT\">OUTPUT</option></select> <input type=\"submit\" value=\"pinMode\"> </form>");

  //Forms for addition/playback of notes, song and instructions
  client.print("<p></p><form action=\"/HTP\"> <label for=\"playNotex\">Play a Note Now  | Frequency (Hz)</label> <input type=\"number\" id=\"playNotex\" name=\"playNotex\" value=\"440\"> <label for=\"x\">Duration (ms)</label> <input type=\"number\" id=\"x\" name=\"x\" value=\"250\"><input type=\"submit\" value=\"Play\"> </form>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"addNotex\">Add Note to Song | Frequency (Hz)</label> <input type=\"number\" id=\"addNotex\" name=\"addNotex\" value=\"440\"> <label for=\"x\">Duration (ms)</label> <input type=\"number\" id=\"x\" name=\"x\" value=\"250\"><input type=\"submit\" value=\"Add\"> </form>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"addInstructionx\">Add a CMD to Song | OpCode</label> <input type=\"number\" id=\"addInstructionx\" name=\"addInstructionx\" value=\"1\"> <label for=\"x\">Arg 1</label> <input type=\"number\" id=\"x\" name=\"x\" value=\"0\"><label for=\"x\">Arg 2</label> <input type=\"number\" id=\"x\" name=\"x\" value=\"0\"><input type=\"submit\" value=\"Add\"> </form>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"pointerLocx\">Song Edit Cursor | Location</label> <input type=\"number\" id=\"pointerLocx\" name=\"pointerLocx\" value=\"0\"> <input type=\"submit\" value=\"Move Cursor\"> </form>");
  client.print("<p></p><form action=\"/HTP\"> <label for=\"playSongx\">Play the Song    | Start Position</label> <input type=\"number\" id=\"playSongx\" name=\"playSongx\" value=\"0\"> <input type=\"submit\" value=\"Play\"> </form>");
  
  // The HTTP response ends with another blank line:
  client.println();
  
}
