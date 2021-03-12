// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/********* Configuration ************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/******** Example Starts Here ***********/

// this int will hold the current count for our sketch
String luz_verde;
String luz_rojo;
String Temperatura;

// Track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds.
//
// Because this sketch is publishing AND subscribing, we can't use a long
// delay() function call in the main loop since that would prevent io.run()
// from being called often enough to receive all incoming messages.
//
// Instead, we can use the millis() function to get the current time in
// milliseconds and avoid publishing until IO_LOOP_DELAY milliseconds have
// passed.
#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *L_VERDE = io.feed("Led_Verde");
AdafruitIO_Feed *L_ROJO = io.feed("Led_Rojo");
AdafruitIO_Feed *T = io.feed("Valor_Temp");

void setup() {

  // start the serial connection
  Serial.begin(9600);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  L_VERDE->onMessage(handleMessage1);
  L_ROJO->onMessage(handleMessage2);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  L_VERDE->get();
  L_ROJO->get();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();



     if (Serial2.read() ==  '#'){
     Temperatura = 'T:';
     Temperatura = Temperatura + Serial2.read();
     Temperatura = Temperatura + Serial2.read();
     Temperatura = Temperatura + char(Serial2.read());
     Temperatura = Temperatura + Serial2.read();
     Temperatura = Temperatura + Serial2.read();
     Temperatura = Temperatura + char(167)+'C';
    }
     T ->save(Temperatura);

}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage1(AdafruitIO_Data *data) {

  luz_verde = data->value();
  if (luz_verde == "ON"){
     Serial.print("ON");
     Serial2.write(1);
  }
  else if (luz_verde == "OFF") {
     Serial.print("OFF");
     Serial2.write(0);
  }

}

void handleMessage2(AdafruitIO_Data *data) {

  luz_rojo = data->value();
    if (luz_rojo == "ON"){
     Serial.print("ON");
     Serial2.write(3);
    }
    else if (luz_rojo == "OFF") {
     Serial.print("OFF");
     Serial2.write(2);
    }

    }
