//-----------------------------------------------------
//variables to compute mean daylight hours using latitude
//-----------------------------------------------------
float p[12], lat, lat_lower, lat_upper, p_lower[12], p_upper[12];
float p_table[13][13] = {
{0,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27,0.27},
{5,0.27,0.27,0.27,0.28,0.28,0.28,0.28,0.28,0.28,0.27,0.27,0.27},
{10,0.26,0.27,0.27,0.28,0.28,0.29,0.29,0.28,0.28,0.27,0.26,0.26},
{15,0.26,0.26,0.27,0.28,0.29,0.29,0.29,0.28,0.28,0.27,0.26,0.25},
{20,0.25,0.26,0.27,0.28,0.29,0.3,0.3,0.29,0.28,0.26,0.25,0.25},
{25,0.24,0.26,0.27,0.29,0.3,0.31,0.31,0.29,0.28,0.26,0.25,0.24},
{30,0.24,0.25,0.27,0.29,0.31,0.32,0.31,0.3,0.28,0.26,0.24,0.23},
{35,0.23,0.25,0.27,0.29,0.31,0.32,0.32,0.3,0.28,0.25,0.23,0.22},
{40,0.22,0.24,0.27,0.3,0.32,0.34,0.33,0.31,0.28,0.25,0.22,0.21},
{45,0.2,0.23,0.27,0.3,0.34,0.35,0.34,0.32,0.28,0.24,0.21,0.2},
{50,0.19,0.23,0.27,0.31,0.34,0.36,0.35,0.32,0.28,0.24,0.2,0.18},
{55,0.17,0.21,0.26,0.32,0.36,0.39,0.38,0.33,0.28,0.23,0.18,0.16},
{60,0.15,0.2,0.26,0.32,0.38,0.41,0.4,0.34,0.28,0.22,0.17,0.13}};
//-----------------------------------------------------
//-----------------------------------------------------


//-----------------------------------------------------
//computation of mean daylight hours
//-----------------------------------------------------
float meanDaytimeHours()
{
  lat = config.latitude;
  Serial.print("\nLatitude of plantation: ");
  Serial.println(lat);
  int lat_upper_location = floor(lat/5)+1;
  int lat_lower_location = floor(lat/5);
  Serial.print("\nUpper location in matrix: ");
  Serial.println(lat_upper_location);
  Serial.print("Lower location in matrix: ");
  Serial.println(lat_lower_location);
  
  lat_upper = lat_upper_location*5;
  lat_lower = lat_lower_location*5;
  Serial.print("\nUpper latitude: ");
  Serial.println(lat_upper);
  Serial.print("Lower latitude: ");
  Serial.println(lat_lower);
  Serial.println();
  
  for(int i=0;i<12;i++)
  {
    p_lower[i] = p_table[lat_lower_location][i+1];
    p_upper[i] = p_table[lat_upper_location][i+1];
  }
  Serial.print("p upper: ");
  for(int i=0;i<12;i++)
  {
    Serial.print(p_upper[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("p lower: ");
  for(int i=0;i<12;i++)
  {
    Serial.print(p_lower[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  Serial.print("p values: ");
  for(int i=0;i<12;i++)
  {
    p[i] = ((lat-lat_upper)*(p_lower[i]-p_upper[i])/(lat_lower-lat_upper))+p_upper[i];
    Serial.print(p[i],4);
    Serial.print(" ");
//    p_nagpur[i] = p[i];
  }
  Serial.println();
  delay(1000);
}
//-----------------------------------------------------







