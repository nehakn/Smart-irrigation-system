//-----------------------------------------------------
//libraries and header files
//-----------------------------------------------------
#include <EEPROM.h>
#include <SPI.h>
#include "SdFat.h"
#include <dht.h>
#include <Wire.h>
#include <RTClib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <WiFiUdp.h> 
#include "helpers.h"
#include "global.h"
#include "Page_Root.h"
#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_Information.h"
#include "PAGE_NetworkConfiguration.h"
#include "Page_trial.h"
#include "cropDays.h"
//#include <DebugMacros.h>
//#include <HTTPSRedirect.h>
//#include "userAlert.h"
#include "pCal.h"
//-----------------------------------------------------

//-----------------------------------------------------
//setup function for eeprom, serial, server and sd begin
//admin mode
//-----------------------------------------------------
void setup(void) 
{
  EEPROM.begin(512);
  delay(1000);
  id = ESP.getChipId();
  String(id).toCharArray(ACCESS_POINT_NAME,15);
  Serial.begin(115200);
  delay(1000);
  if (!SD.begin(SD_CS, SPI_HALF_SPEED)) 
  {
    Serial.println("Failed to begin SD Card");
  }
  displaySDData();
  
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));    
  }

  config.ssid = mySsid;
  config.password = myPassword;
  config.dhcp = true;
  config.IP[0] = 192;config.IP[1] = 168;config.IP[2] = 4;config.IP[3] = 1;
  config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
  config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;
  // config.ntpServerName = "in.pool.ntp.org";
  // config.Update_Time_Via_NTP_Every =  0;
  // config.timezone = +5.5;
  // config.daylight = true;
  // config.DeviceName = " ";
  // config.loginid = " ";
  // config.loginpassword = " ";
  DateTime cropDays = rtc.now();
  config.cropName = "Beans";
  config.dateOfPlantation[0] = cropDays.day();
  config.dateOfPlantation[1] = cropDays.month();
  config.dateOfPlantation[2] = cropDays.year();
  config.soilType = "Shallow";
  config.latitude = 22;
  // WriteConfig();

  if(!ReadConfig())
  {
    config.cropName = "Beans";
    config.dateOfPlantation[0] = cropDays.day();
    config.dateOfPlantation[1] = cropDays.month();
    config.dateOfPlantation[2] = cropDays.year();
    config.soilType = "Shallow";
    config.latitude = 22;
    WriteConfig();
  }
  Serial.println("General config applied");

  if (AdminEnabled)
  {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ACCESS_POINT_NAME );
  }
  else
  {
    WiFi.mode(WIFI_STA);
  }
  
  server.on ( "/", []() 
  {
    Serial.println("admin.html");
    server.send ( 200, "text/html", PAGE_AdminMainPage );
  }  );
  server.on ( "/favicon.ico",   []() 
  {
    Serial.println("favicon.ico");
    server.send ( 200, "text/html", "" );
  }  );
  server.on ( "/config.html", send_network_configuration_html );
  server.on ( "/info.html", []() 
  {
    Serial.println("info.html");
    server.send ( 200, "text/html", PAGE_Information );
  }  );
  server.on ( "/cropSel.html", send_trial_html  );
  server.on ( "/style.css", []() 
  {
    Serial.println("style.css");
    server.send ( 200, "text/plain", PAGE_Style_css );
  } );
  server.on ( "/microajax.js", []() 
  {
    Serial.println("microajax.js");
    server.send ( 200, "text/plain", PAGE_microajax_js );
  } );
  server.on ( "/admin/values", send_network_configuration_values_html );
  server.on ( "/admin/connectionstate", send_connection_state_values_html );
  server.on ( "/admin/infovalues", send_information_values_html );
  server.on ( "/admin/cropSel", send_trial_values_html);

  server.onNotFound ([](){Serial.println("Page Not Found"); 
  server.send (400, "text/html", "Page not Found" );});
  server.begin();
  Serial.println("HTTP server started");
  tkSecond.attach(1,Second_Tick);
}
//-----------------------------------------------------

//-----------------------------------------------------
//one time setup mode
//-----------------------------------------------------
void loop (void) 
{
  if (AdminEnabled)
  {
    if (AdminTimeOutCounter > AdminTimeOut)
    {
      AdminEnabled = false;
      Serial.println("Admin Mode disabled!");
      WiFi.mode(WIFI_STA);
      MqttClient.setServer(broker, 1883);
      meanDaytimeHours();
      returnCropParam();
      ComputeETc();
      ComputeINUsingETc();
      IrrigationDatesUsingETc();
//      SendIrrigationDatesToSD();
      topicNames();
    }
  }

  server.handleClient();

  if(!AdminEnabled)
  {
    ReadSensorData();
//    Serial.println("Hello");
    delay(15000);
  }
}
//-----------------------------------------------------

//-----------------------------------------------------
//the continuous monitoring mode of USP
//-----------------------------------------------------
int ReadSensorData()
{
  /* assign water needed variable as per the dates to irrigate */
  dateReadFunc = rtc.now();
  Serial.print("\nDate today: ");
  Serial.print(dateReadFunc.day());
  Serial.print("/");
  Serial.print(dateReadFunc.month());
  Serial.print("/");
  Serial.println(dateReadFunc.year());
  Serial.print("\nWindow size: ");
  Serial.println(window); 
  Serial.print("\nCounter");
  Serial.print("\t");
  Serial.print("Pg");
  Serial.print("\t");
  Serial.print("Temp");
  Serial.print("\t");
  Serial.print("Humi");
  Serial.print("\t");
  Serial.println("SM");
//  sendValueToGoogle();
  Serial.println("Data written");
  /* data sensing */
  for(counter=0;counter<64;counter++)
  { 
    /* get the sensor data */
    int chk = DHT.read11(DHT11_PIN);
    mat[counter][0] = DHT.temperature;
    mat[counter][1] = DHT.humidity;
    mat[counter][2] = analogRead(soilMoisturePin);
    pg = 1;
    if(boolString[dateTimeToString(mid) <= dateTimeToString(dateReadFunc) &&  dateTimeToString(dateReadFunc) <= dateTimeToString(dev)] == "true");
    {
      pg = 1;
    }
    if(boolString[dateTimeToString(dev) <= dateTimeToString(dateReadFunc) &&  dateTimeToString(dateReadFunc) <= dateTimeToString(fin)] == "true");
    {
      pg = 2;
    }
    if(boolString[dateTimeToString(fin) <= dateTimeToString(dateReadFunc) &&  dateTimeToString(dateReadFunc) <= dateTimeToString(fin_end)] == "true");
    {
      pg = 3;
    }
    mat[counter][3] = pg;
    Serial.print(counter);
    Serial.print("\t");
    Serial.print(mat[counter][3]);
    Serial.print("\t");
    Serial.print(mat[counter][0]);
    Serial.print("\t");
    Serial.print(mat[counter][1]);
    Serial.print("\t");
    Serial.print(mat[counter][2]);
    Serial.print("\t");
    Serial.println();
                  
    /* sending bunch of values and to IU for mode decision */
    if(check_irrigation_status_nm==1 && stopWaterForToday==0)
    {
      Serial.println("Normal mode in process\n");
      IrrigateUsingETc();
    }

    if(check_irrigation_status_im==1 && stopWaterForToday==0)
    {
      Serial.println("Interrupt mode in process");
      IrrigateUsingNNDecision(1);
    }

    /* if no mode is still selected, checking is done everyday */
    if(check_irrigation_status_nm==0 && check_irrigation_status_im==0 && stopWaterForToday==0)
    {
      if((counter+1)%window==0)
      {
        Serial.println("Send the data for mode selection");
        mode_decision = ModeDecision();
        /* selection of decision making after decision mode select is obtained */
        if(mode_decision==1)
        {
          Serial.println("\nMode decision received, Irrigating using NN");
          NNDecision();
        }
        else  // normal ETc mode
        {
          Serial.println("\nMode decision received, Irrigating using ETc");
          IrrigateUsingETc();
        }
      }
    }

    if(stopWaterForToday==1)
    {
      Serial.println("Water already supplied for today");
    }

    /* when current day ends, time set by user initially */
    if(dateForStopWaterIni.day() == dateReadFunc.day() && dateForStopWaterIni.month() == dateReadFunc.month()
    && check_irrigation_status_nm==0 && check_irrigation_status_im==0)      
    {
      stopWaterForToday = 0;
      Serial.print("\nStop water initialized to 0");
      // Serial.println(stopWaterForToday);
    }
    delay(3000); 
  }
  Serial.println("<--------------------------->");
  MqttClient.disconnect();
  if (!MqttClient.connected()) 
  {
    reconnect();
  }
  MqttClient.loop(); 
  
  long now = millis();
  if (now - lastMsg > 5000) 
  {
    lastMsg = now;
    snprintf (msg, 75, "%ld", mat[counter-1][0]);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MqttClient.publish(tempTopic, msg);
    snprintf (msg, 75, "%ld", mat[counter-1][1]);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MqttClient.publish(humidTopic, msg);
    snprintf (msg, 75, "%ld", mat[counter-1][2]);
    Serial.print("Publish message: ");
    Serial.println(msg);
    MqttClient.publish(smTopic, msg);
  }
  Serial.println();
//  delay(60000);  
}
//-----------------------------------------------------

//-----------------------------------------------------
//nn or etc selection
//in continuous monitoring mode
//-----------------------------------------------------
int ModeDecision()
{
  mode_decision = 0;
  Serial.println("Mode decision: ");
//  added on jan 25, 2019
//  if(dateReadFunc.day()==dateForStopWaterIni.day())
//  {
//    
//  }

  for(int rev_counter=counter;rev_counter>=counter-7;rev_counter--)
  {
    /* ambient temperature and soil moisture */
//    if(mat[rev_counter][2]<300&&mat[rev_counter][0]>29 | mat[rev_counter][2]>600)
    if(mat[rev_counter][2]>600)
    {
      mode_decision = mode_decision + 1;
    }
//    if(mat[rev_counter][2]<300&&mat[rev_counter][0]>29)
    if(mat[rev_counter][2]<300) // for checking only
    {
      mode_decision = mode_decision + 1;
    }
    else
    {
      mode_decision = mode_decision;
    }
    Serial.println(mode_decision);
  }
  mode_decision = round(mode_decision/8);
  Serial.print("Selected mode is: ");
  if(mode_decision==0)
  {
    Serial.print(mode_decision);
    Serial.print(", ");
    Serial.println("Normal mode using ETc");
  }
  else
  {
    Serial.print(mode_decision);
    Serial.print(", ");
    Serial.println("Interrupt mode using NN");
  }
//  sending the selected mode to broker
  MqttClient.disconnect();
  if (!MqttClient.connected()) 
  {
    reconnect();
  }
  MqttClient.loop(); 
  snprintf(message, 75, "%ld", mode_decision);
  MqttClient.publish(modeTopic, message);
  return mode_decision;
}
//-----------------------------------------------------

//-----------------------------------------------------
//if etc based irrigation is selected
//in continuous monitoring mode
//-----------------------------------------------------
void IrrigateUsingETc()
{
  /* motor on duration and gap for current month */
  DateTime now = rtc.now();
  // delay_time = rtc.now();
//   dateForStopWaterIni = rtc.now();
  for(int i=0;i<no_of_applications+1;i++)
  {
    Serial.print(dates_to_irrigate[i+1][1]);   
    Serial.print('/');
    Serial.print(dates_to_irrigate[i+1][2]);
    if(String(now.day())==dates_to_irrigate[i+1][1] && String(now.month())==dates_to_irrigate[i+1][2] && 
     check_irrigation_status_nm==0)
    {
      Serial.println(": Correct day or condition to water");
      check_irrigation_status_nm = 1;
      delay_time =  now + TimeSpan(0,0,1,0);
//      delay_time =  now + TimeSpan(0,0,delay_month[now.month()-1],0);
      dateForStopWaterIni = now + TimeSpan(1,0,0,0);
      IURequest(1);
      Serial.print("Delay time: ");
      Serial.print(delay_time.day());
      Serial.print("/");
      Serial.print(delay_time.month());
      Serial.print("/");
      Serial.print(delay_time.year());
      Serial.print("\t");
      Serial.print(delay_time.hour());
      Serial.print(':');
      Serial.println(delay_time.minute());
      
      Serial.print("Current time: ");
      Serial.print(now.day());
      Serial.print("/");
      Serial.print(now.month());
      Serial.print("/");
      Serial.print(now.year());
      Serial.print("\t");
      Serial.print(now.hour());
      Serial.print(':');
      Serial.println(now.minute());
    }
    else
    {
      Serial.println(": Not the correct day or condition to water");
      dateForStopWaterIni = now + TimeSpan(1,0,0,0);
    }
  }

  delay_time = delay_time;
  dateForStopWaterIni = dateForStopWaterIni;
  
  Serial.print("DateForStopWaterIni: ");
  Serial.print(dateForStopWaterIni.day());
  Serial.print('/');
  Serial.println(dateForStopWaterIni.month());
  Serial.println();

  /* for turning off the motor */
  if(check_irrigation_status_nm==1&&delay_time.minute()<=now.minute()&&delay_time.hour()<=now.hour())
  {
    Serial.println("Time over, turn off the motor");
    check_irrigation_status_nm = 0;
    IURequest(0); 
    stopWaterForToday = 1;
  }
  delay(1000);
}
//-----------------------------------------------------

//-----------------------------------------------------
//computation of dates using etc for storing on SIU
//during one time setup mode
//-----------------------------------------------------
void IrrigationDatesUsingETc()
{
  DateTime future;
  DateTime dateTimeNow = rtc.now();
  DateTime dateTime (Vyear_i,Vmonth_i,Vdate_i);
  Serial.print("Date today: ");
  Serial.print(dateTimeNow.day());
  Serial.print("/");
  Serial.print(dateTimeNow.month());
  Serial.print("/");
  Serial.print(dateTimeNow.year());
  Serial.print(" ");
  Serial.print(dateTimeNow.hour());
  Serial.print(":");
  Serial.print(dateTimeNow.minute());
  Serial.print(":");
  Serial.println(dateTimeNow.second());
//  time_t utcCalc;
//  int current_month_by_rtc;
  dates_to_irrigate[no_of_applications + 2][3];
  Serial.println("Irrigating dates: ");
  int sum_date_check = 0, month_check = Vmonth_i;
//  int month_check = current_month_by_rtc;
//  for (int i = 0; i < no_of_applications+1; i++)
  for (int i = 0; i < no_of_applications_modified+1; i++)
  {
    future = dateTime + TimeSpan(sum_date_check, 0, 0, 0);
    sum_date_check = sum_date_check + irrigation_interval_days_final[month_check - 1];
    // Serial.print(sum_date_check);
    // Serial.print(" ");
    // Serial.print(month_check);
    // Serial.println();
    // future = dateTime + TimeSpan(sum_date_check, 0, 0, 0);
    Serial.print(future.day(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.println(future.year(), DEC);
    // Serial.print(' ');
    // Serial.print(future.hour(), DEC);
    // Serial.print(':');
    // Serial.print(future.minute(), DEC);
    // Serial.print(':');
    // Serial.println(future.second(), DEC);

    dates_to_irrigate[i + 1][1] = future.day();
    dates_to_irrigate[i + 1][2] = future.month();
    dates_to_irrigate[i + 1][3] = future.year();
    month_check = int(future.month());
  }
  delay(5000);
}
//-----------------------------------------------------

//-----------------------------------------------------
//nn based decision making
//in continuous monitoring mode
//-----------------------------------------------------
/* decision making for watering the crop */
void NNDecision()
{
  Serial.println("\nTaking decision to water or not using NN");
  // weights and biases after training
  /* declaration */
  float i1, i2, i3, i4, o1;
  float h1, h2, h3, h4, h5, h6, h7, h8, h9, h10;
  /*float w1_i1, w2_i1, w3_i1, w4_i1, w5_i1, w6_i1, w7_i1, w8_i1, w9_i1, w10_i1;
  float w1_i2, w2_i2, w3_i2, w4_i2, w5_i2, w6_i2, w7_i2, w8_i2, w9_i2, w10_i2;
  float w1_i3, w2_i3, w3_i3, w4_i3, w5_i3, w6_i3, w7_i3, w8_i3, w9_i3, w10_i3;
  float w1_i4, w2_i4, w3_i4, w4_i4, w5_i4, w6_i4, w7_i4, w8_i4, w9_i4, w10_i4;
  float w1_h1, w1_h2, w1_h3, w1_h4, w1_h5, w1_h6, w1_h7, w1_h8, w1_h9, w1_h10;
  float bh1, bh2, bh3, bh4, bh5, bh6, bh7, bh8, bh9, bh10;
  float bo1;*/
  
  /* initialization */
  float w1_i1 = -25.50649678, w2_i1 = -3.054530104, w3_i1 = -4.17867944939813, w4_i1 = 2.573733731, w5_i1  =2.982645274,
  w6_i1 = -0.266877736, w7_i1 = 2.05235053, w8_i1 = -14.27593001, w9_i1 = 2.491922091, w10_i1 = -2.717879169;
  float w1_i2 = -0.399986735, w2_i2 = 0.097680392, w3_i2 = -1.010984731, w4_i2 = -0.602015358, w5_i2 = -1.490849749,
  w6_i2 = -1.060168526, w7_i2 = -2.283592582, w8_i2 = -0.728935813, w9_i2 = -1.170354732, w10_i2 = -2.288162495;
  float w1_i3 = 22.2981374, w2_i3 = 1.721352828, w3_i3 = 0.161833124, w4_i3 = 0.231396816, w5_i3 = 1.779019052, 
  w6_i3 = 0.402191551,  w7_i3 = 2.118133126, w8_i3 = -6.757794368,  w9_i3 = 0.143732876, w10_i3 = 3.360534173;
  float w1_i4 = 11.03415964,  w2_i4 = -2.260503244,  w3_i4 = -0.627210147,  w4_i4 = -4.115638666,  w5_i4 = -0.440478814,  
  w6_i4 = 2.302437685, w7_i4 = -2.328606487,  w8_i4 = -13.95816952,  w9_i4 = -2.965746322, w10_i4 = 2.351953576;
  float w1_h1 = 1.076110451,  w1_h2 = -2.518733081,  w1_h3 = 1.442184933, w1_h4 = 2.18098851,  w1_h5 = 1.423361757, 
  w1_h6 = -0.256229875,  w1_h7 = -0.83154888, w1_h8 = -1.1191242,  w1_h9 = -1.430491745,  w1_h10 = 0.81134732;
  float bh1 = 25.24971422, bh2 = 3.980778009, bh3 = 1.550305414, bh4  = 2.034893698, bh5 = -2.095238913,
  bh6 = 0.742840189, bh7 = 3.156361848, bh8 = -15.42730539, bh9 = 1.677762108, bh10 = -4.252412939;
  float bo1 = 2.281813833;
    
  i1 = mat[counter][3]/4;   // plant growth 0, 1, 2, 3 from stages
  i2 = mat[counter][0]/50;  // normalized ambient temperature
  i3 = mat[counter][1]/100; // normalized ambient humidity
  i4 = mat[counter][2]/900; // normalized soil moisture

  h1 = bh1 + (i1*w1_i1 + i2*w1_i2 + i3*w1_i3 + i4*w1_i4);
  h2 = bh2 + (i1*w2_i1 + i2*w2_i2 + i3*w2_i3 + i4*w2_i4);
  h3 = bh3 + (i1*w3_i1 + i2*w3_i2 + i3*w3_i3 + i4*w3_i4);
  h4 = bh4 + (i1*w4_i1 + i2*w4_i2 + i3*w4_i3 + i4*w4_i4);
  h5 = bh5 + (i1*w5_i1 + i2*w5_i2 + i3*w5_i3 + i4*w5_i4);
  h6 = bh6 + (i1*w6_i1 + i2*w6_i2 + i3*w6_i3 + i4*w6_i4);
  h7 = bh7 + (i1*w7_i1 + i2*w7_i2 + i3*w7_i3 + i4*w7_i4);
  h8 = bh8 + (i1*w8_i1 + i2*w8_i2 + i3*w8_i3 + i4*w8_i4);
  h9 = bh9 + (i1*w9_i1 + i2*w9_i2 + i3*w9_i3 + i4*w9_i4);
  h10 = bh10 + (i1*w10_i1 + i2*w10_i2 + i3*w10_i3 + i4*w10_i4);
  
  o1 = bo1 + (h1*w1_h1 + h2*w1_h2 + h3*w1_h3 + h4*w1_h4 + h5*w1_h5 
  + h6*w1_h6 + h7*w1_h7 + h8*w1_h8 + h9*w1_h9 + h10*w1_h10);
  Serial.println(o1);
  // Serial.print("\t");

//   if (0<o1<2700) // current target threshold for drip 
   if(o1>51)
   {
     o1 = 0;
     check_irrigation_status_im = 0;
     IrrigateUsingNNDecision(o1);
   }
   else
   {
     o1 = 1;
     IrrigateUsingNNDecision(o1);
   }

//  IrrigateUsingNNDecision(1);
 
   Serial.println(o1);
  delay(1000);
  
}
//-----------------------------------------------------

//-----------------------------------------------------
//used when irrigation using nn is selected
//in continuous monitoring mode
//-----------------------------------------------------
void IrrigateUsingNNDecision(int waterIn)
{
  Serial.println("\nNow irrigating using NN and sending request");
  /* to the IU */
  Serial.print("Irrigating using NN: ");
  Serial.println(waterIn);
  DateTime now = rtc.now();
  if(waterIn==1 && check_irrigation_status_im==0)
  {
    delay_time =  now + TimeSpan(0,0,1,0);
//    delay_time = now + TimeSpan(0,0,delay_month[now.month()-1,0]);
    dateForStopWaterIni = now + TimeSpan(1,0,0,0);
    IURequest(1);
    check_irrigation_status_im = 1;
    Serial.print("\nDelay time ends at: ");
    Serial.print(delay_time.hour());
    Serial.print(':');
    Serial.println(delay_time.minute());
    Serial.print("Current time: ");
    Serial.print(now.hour());
    Serial.print(':');
    Serial.println(now.minute());
  }
//  Serial.print("\nDelay time ends at: ");
//  Serial.print(delay_time.hour());
//  Serial.print(':');
//  Serial.println(delay_time.minute());
//  Serial.print("Current time: ");
//  Serial.print(now.hour());
//  Serial.print(':');
//  Serial.println(now.minute());
  Serial.print("Date until next irrigation: ");
  Serial.print(dateForStopWaterIni.day());
  Serial.print('/');
  Serial.println(dateForStopWaterIni.month());

  /* for turning off the motor */
  if(waterIn==1 && check_irrigation_status_im==1&&delay_time.hour()<=now.hour()&&delay_time.minute()<=now.minute())
  {
    Serial.println("Time over, turn off the motor");
    IURequest(0); 
    check_irrigation_status_im = 0;
    stopWaterForToday = 1;
  }

  /* if rain comes then stop water has to be set to 1 */
  if(waterIn==0)
  {
    Serial.println("\nNo need for water, overlogged!!!");
//    check_irrigation_status_im = 1;
    check_irrigation_status_im = 0;
    stopWaterForToday = 1;
    Serial.println("NN decision: do not water!!");
    dateForStopWaterIni = now + TimeSpan(1,0,0,0);;
  }
  delay(1000);
}
//-----------------------------------------------------

//-----------------------------------------------------
//sending irrigation request to IU
//in continuous monitoring mode
//-----------------------------------------------------
void IURequest(int val)
{
  int id = ESP.getChipId();
  Serial.println("\nCommunicating to IU");
//  IUClient.stop();
//  while (!IUClient.connect(IUServer, IUServerPort))
//  {
//    Serial.println("trying to communicate");
//    delay(5000);
//  }
//  Serial.print("CONNECTED, Sending: ");
//  Serial.println(val);
//  Serial.println();
//  IUClient.println(String(id) + String(val));
  /* the sent value 0/1 */  
  MqttClient.disconnect();
  if (!MqttClient.connected()) 
  {
    reconnect();
  }
  MqttClient.loop(); 
  snprintf(message, 75, "%ld", (val));
  MqttClient.publish(irriTopic, message);
//  sendValueToGoogle(String(val));
  delay(1000);
}
//-----------------------------------------------------

//-----------------------------------------------------
//computation of etc during one time setup
//-----------------------------------------------------
void ComputeETc() 
{
  Serial.println("\nComputing the ETc values for a crop season");
  Serial.print("ETo");
  Serial.print("\t");
  Serial.print("ETc mm/day");
  Serial.print("\t");
  Serial.println("ETc mm/month");
  for(int i=0;i<12;i++)
  {
    t_mean[i] = (t_min[i] + t_max[i]) * 0.5;
  }
  for (int i = 0; i < 12; i++)
  {
    eto[i] = p[i] * (0.46 * t_mean[i] + 8.128);
    etc_mmperday[i] = kc[i] * eto[i];
    etc_mmpermonth[i] = etc_mmperday[i] * 30;
    Serial.print(eto[i]);
    Serial.print("\t");
    Serial.print(etc_mmperday[i]);
    Serial.print("\t\t");
    Serial.println(etc_mmpermonth[i]);
  }
  Serial.println();
  delay(5000);
}
//-----------------------------------------------------

//-----------------------------------------------------
//IN needs computation after dnet and kc inclusion
//during one time setup
//-----------------------------------------------------
void ComputeINUsingETc()
{
  float rainfall_effective[12], irrigation_mmpermonth[12], irrigation_mmperday[12];
  float irrigation_totalseason;
  int months_to_irrigate = 0;
  int d_net_permonth[12], d_net_permonthIN[12], months_inc[12];
  Serial.println("Computing IN and d net for growing season");
  /* effective rainfall Pe rainfall_effective */
  Serial.print("Pe");
  Serial.print("\t");
  Serial.println("IN mm/month");
  for (int i = 0; i < 12; i++)
  {
    if (etc_mmperday[i] > 0)
    {
      if (preci[i] > 75)
      {
        rainfall_effective[i] = 0.8 * preci[i] - 25;
      }
      else
      {
        rainfall_effective[i] = 0.6 * preci[i] - 10;
      }
    }
    else
    {
      rainfall_effective[i] = 0;
    }
    /* IN mm/month the irrigation irrigation_mmpermonth and irrigation_mmperday */
    for (int i = 0; i < 12; i++)
    {
      if (etc_mmpermonth[i] - rainfall_effective[i] < 0)
      {
        irrigation_mmpermonth[i] = 0;
        irrigation_mmperday[i] = 0;
      }
      else
        irrigation_mmpermonth[i] = etc_mmpermonth[i] - rainfall_effective[i];
      irrigation_mmperday[i] = irrigation_mmpermonth[i] / 30;
    }
    Serial.print(rainfall_effective[i]);
    Serial.print("\t");
    Serial.println(irrigation_mmpermonth[i]);
  }
  Serial.println();

  /* total IN, months and days to irrigate */
  for (int i = 0; i < 12; i++)
  {
    //irrigation_totalseason = irrigation_mmpermonth[i]+0;
    if (irrigation_mmpermonth[i] > 0)
    {
      months_to_irrigate = months_to_irrigate + 1;
    }
    else
    {
      months_to_irrigate = months_to_irrigate;
    }
  }
  for (int i = 0; i < 12; i++)
  {
    irrigation_totalseason = irrigation_mmpermonth[i] + irrigation_totalseason;
  }
  Serial.print("Total irrigation needed: ");
  Serial.println(irrigation_totalseason);
  Serial.print("Total no. of days in growing season: ");
  Serial.println(growing_days);
  Serial.print("Total months to irrigate: ");
  Serial.println(months_to_irrigate);

  /* computation for total irrigation applications, gaps
      at which irrrigation is to be provided and d net and d net-IN */
  no_of_applications = ceil(irrigation_totalseason / d_net_crop);
  int irrigation_interval_days = floor(growing_days / no_of_applications);
  Serial.print("d net for crop: ");
  Serial.println(d_net_crop);
  Serial.print("No. of irrigation applications: ");
  Serial.println(no_of_applications);
  Serial.print("Gap interval for irrigation applications in day: ");
  Serial.println(irrigation_interval_days);

  /* d net and d net-IN */
  Serial.print("\nd net mm/month");
  Serial.print("\t");
  Serial.print("d net-IN mm/month");
  Serial.print("\t");
  Serial.println("Months inc");
  for (int i = 0; i < 12; i++)
  {
    // if (irrigation_mmpermonth[i] > 0)
    if (etc_mmpermonth[i] > 0)
    {
      d_net_permonth[i] = (30 * d_net_crop) / irrigation_interval_days;
      d_net_permonthIN[i] = d_net_permonth[i] - irrigation_mmpermonth[i];
      months_inc[i] = 1;
    }
    else
    {
      d_net_permonth[i] = 0;
      d_net_permonthIN[i] = 0;
      months_inc[i] = 0;
    }
    Serial.print(d_net_permonth[i]);
    Serial.print("\t");
    Serial.print(d_net_permonthIN[i]);
    Serial.print("\t");
    Serial.println(months_inc[i]);
  }

  /* variables used for adjustment for the peak irrigation season */
  float d_net_for_peak_season[12], days_for_peak = 0, d_net_IN_peak[12], d_net_IN_final[12];
  float d_net_for_peak_season_sum = 0, irrigation_interval_peak = 0;;

  /* d net for peak season */
  Serial.println("\nd net for peak season: ");
  for (int i = 0; i < 12; i++)
  {
    if (d_net_permonthIN[i] > 0)
    {
      d_net_for_peak_season[i] = 0;
    }
    else
    {
      d_net_for_peak_season[i] = irrigation_mmpermonth[i];
    }
    Serial.println(d_net_for_peak_season[i]);
  }

  /* computing the peak days */
  for (int i = 0; i < 12; i++)
  {
    d_net_for_peak_season_sum = d_net_for_peak_season_sum + d_net_for_peak_season[i];
    if (d_net_for_peak_season[i] == 0)
    {
      days_for_peak = days_for_peak;
    }
    else
    {
      days_for_peak = days_for_peak + 30;
    }
  }
  Serial.print("\nDays for irrigation in peak season: ");
  Serial.println(days_for_peak);
  no_of_applications_modified = ceil(d_net_for_peak_season_sum / d_net_crop);
  Serial.print("No. of applications modified: ");
  Serial.println(no_of_applications_modified);
  irrigation_interval_peak = floor(days_for_peak / no_of_applications_modified);
  int water_total = no_of_applications_modified * d_net_crop;
  int water_adjust = water_total / months_to_irrigate;
  Serial.print("Total water modified: ");
  Serial.println(water_total);
  Serial.print("Water adjustment: ");
  Serial.println(water_adjust);

  /* d net-IN for peak and final */
  Serial.println("\nd net-IN peak and final");
  for (int i = 0; i < 12; i++)
  {
    if (d_net_for_peak_season[i] == 0)
    {
      d_net_IN_peak[i] = 0;
      d_net_IN_final[i] = d_net_permonthIN[i];
    }
    else
    {
      d_net_IN_peak[i] = d_net_permonthIN[i] + water_adjust;
      d_net_IN_final[i] = d_net_IN_peak[i];
    }
    Serial.print(d_net_IN_peak[i]);
    Serial.print("\t");
    Serial.println(d_net_IN_final[i]);
  }

  /* final irrigation interval as per seasons in one whole season */
  for (int i = 0; i < 12; i++)
  {
    if (months_inc[i] != 0)
    {
      if (d_net_for_peak_season[i] == 0)
      {
        irrigation_interval_days_final[i] = irrigation_interval_days;
      }
      else
      {
        irrigation_interval_days_final[i] = irrigation_interval_peak;
      }
    }
    else
    {
      irrigation_interval_days_final[i] = 0;
    }
  }

  /* printing the IN interval computed */
  Serial.println("\nFinal IN interval");
  for (int i = 0; i < 12; i++)
  {
    Serial.println(irrigation_interval_days_final[i]);
  }

  /* the water computation part */
  /* the gross irrigation depth in mm */
  float d_gross_crop = (100 * d_net_crop) / 90;
  Serial.print("\nGross irrigation depth in mm: ");
  Serial.println(d_gross_crop);
  /* water flow in mm/day */
  float water_flow_mmperday = d_gross_crop / irrigation_interval_days;
  Serial.print("Water flow in mm/day: ");
  Serial.println(water_flow_mmperday);
  /* water flow in l/sec/area */
  float water_flow_lsecarea = (water_flow_mmperday / 8.64);
  Serial.print("Water flow in l/sec/area: ");
  Serial.println(water_flow_lsecarea);
  /* field area in hectare */
//  int field_area = 1;
  float field_area = pot_size;
  Serial.print("Crop planted area (sq. inch): ");
  Serial.println(field_area);
  /* water for wanted field area */
  float water_for_field_area = water_flow_lsecarea * field_area;
  Serial.print("Water for field area: ");
  Serial.println(water_for_field_area);
  /* net application rate of water, depends upon motor */
  int net_app_rate = 2;
  /* runtime of motor in min/day */
  runtime_minperday = (water_flow_mmperday * 60) / net_app_rate;
  Serial.print("Runtime for field area in min/day: ");
  Serial.println(runtime_minperday);
  /* runtime in hr/day */
  runtime_hourperday = ceil(water_flow_mmperday / net_app_rate);
  Serial.print("Runtime for field area in hr/day: ");
  Serial.println(runtime_hourperday);
  /* computation for total period */
  Serial.println("Irrigation on time:");
  for(int i=0;i<12;i++)
  {
    delay_month[i] = runtime_minperday*etc_mmperday[i];
    Serial.print(name_months[i]);
    Serial.print("\t");
    Serial.println(delay_month[i]);
  }
  Serial.println();
  delay(5000);
}
//-----------------------------------------------------
