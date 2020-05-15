
/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
Adafruit_INA219 ina219_load(0x41);

// set up the 'voltage' and 'current' feeds
AdafruitIO_Feed *voltage = io.feed("solar.solar-panel-voltage");
AdafruitIO_Feed *power = io.feed("solar.solar-panel-power");
AdafruitIO_Feed *current = io.feed("solar.solar-panel-current");
//AdafruitIO_Feed *voltage_load = io.feed("solar.solar-panel-voltage-load");
AdafruitIO_Feed *power_load = io.feed("solar.solar-panel-power-load");
AdafruitIO_Feed *current_load = io.feed("solar.solar-panel-current-load");

void setup() {

  // start the serial connection
  Serial.begin(115200);
  // wait for serial monitor to open
  while(! Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    // while (1) { delay(10); }
  }
  if (! ina219_load.begin()) {
    Serial.println("Failed to find INA219 load chip");
    // while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  
  Serial.println("Measuring voltage and current with INA219 ...");
}

void loop() {

    // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(15000);
  //  delay(1000);

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // float voltage_value = 12.0;
  
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  float power_W = 0.0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  power_W = power_mW/1000.0;
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  current->save(current_mA); 
  power->save(power_W);
  voltage->save(loadvoltage);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  float shuntvoltage_load = 0;
  float busvoltage_load = 0;
  float current_mA_load = 0;
  float loadvoltage_load = 0;
  float power_mW_load = 0;
  float power_W_load = 0.0;

  shuntvoltage_load = ina219_load.getShuntVoltage_mV();
  busvoltage_load = ina219_load.getBusVoltage_V();
  current_mA_load = ina219_load.getCurrent_mA();
  power_mW_load = ina219_load.getPower_mW();
  power_W_load = power_mW_load/1000.0;
  loadvoltage_load = busvoltage_load + (shuntvoltage_load / 1000);

  current_load->save(current_mA_load); 
  power_load->save(power_W_load);
//  voltage_load->save(loadvoltage_load);
  
  Serial.print("Bus Voltage Load:   "); Serial.print(busvoltage_load); Serial.println(" V");
  Serial.print("Shunt Voltage Load: "); Serial.print(shuntvoltage_load); Serial.println(" mV");
  Serial.print("Load Voltage Load:  "); Serial.print(loadvoltage_load); Serial.println(" V");
  Serial.print("Current Load:       "); Serial.print(current_mA_load); Serial.println(" mA");
  Serial.print("Power Load:         "); Serial.print(power_mW_load); Serial.println(" mW");
  Serial.println("");


  //delay(2000);

}
