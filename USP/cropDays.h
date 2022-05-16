//-----------------------------------------------------
// for d net of crop and # days in stages
//-----------------------------------------------------
int initialstage_days, cropdevstage_days, midseasonstage_days, finalstage_days;
int d_net_crop, growing_days;
int Vdate_i, Vmonth_i, Vyear_i;
DateTime ini, mid, dev, fin, fin_end;
String name_months[12] = {"Jan","Feb","Mar","Apr","May","June","July","Aug","Sept","Oct","Nov","Dec"};
int days_in_months[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int month_loc[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
int days_avail[12];
int month_ini_days[12], month_dev_days[12], month_mid_days[12], month_final_days[12];
int days_dist[4][12];
float kc[12], kc_stages[4];
//-----------------------------------------------------
//-----------------------------------------------------

//-----------------------------------------------------
//display of date used in USP
//-----------------------------------------------------
void showDate(const DateTime& dt, const char* txt) 
{
    Serial.print(txt);
    Serial.print(' ');
    Serial.print(dt.day(), DEC);
    Serial.print('/');
    Serial.print(dt.month(), DEC);
    Serial.print('/');
    Serial.println(dt.year(), DEC);
}
//-----------------------------------------------------

//-----------------------------------------------------
//to get kc and other values for selected crop
//-----------------------------------------------------
void returnCropParam()
{
  if(Vcrop=="Normal Wheat" | Vcrop=="Rough Wheat" | Vcrop=="Rye" | Vcrop=="Barley" | Vcrop=="Oat")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 60;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 15;
    cropdevstage_days = 30;
    midseasonstage_days = 50;
    finalstage_days = 30;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    if(Vcrop=="Normal Wheat")
    {
      kc_stages[0] = 0.4;
      kc_stages[1] = 0.8;
      kc_stages[2] = 1.15;
      kc_stages[3] = 0.7;
    }
    if(Vcrop=="Rough Wheat")
    {
      kc_stages[0] = 0.3;
      kc_stages[1] = 0.65;
      kc_stages[2] = 0.95;
      kc_stages[3] = 0.7;
    }
    else
    {
      kc_stages[0] = 0.35;
      kc_stages[1] = 0.75;
      kc_stages[2] = 1.15;
      kc_stages[3] = 0.45;
    }
  }

  if(Vcrop=="Corn")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 55;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 15;
    cropdevstage_days = 30;
    midseasonstage_days = 50;
    finalstage_days = 30;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.75;  
    kc_stages[2] = 1.15; 
    kc_stages[3] = 0.45;
  }

  if(Vcrop=="Maize" | Vcrop=="Rice")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 55;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 25;
    cropdevstage_days = 30;
    midseasonstage_days = 45;
    finalstage_days = 60;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.4;
    kc_stages[1] = 0.8;
    kc_stages[2] = 1.15;
    kc_stages[3] = 1;
  }

  if(Vcrop=="Peas")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 25;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 45;
    }    
    initialstage_days = 15;
    cropdevstage_days = 30;
    midseasonstage_days = 35;
    finalstage_days = 15;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.45;
    kc_stages[1] = 0.8;
    kc_stages[2] = 1.15;
    kc_stages[3] = 1.05;
  }

  if(Vcrop=="Beans")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 15;
    cropdevstage_days = 30;
    midseasonstage_days = 30;
    finalstage_days = 10;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.7;
    kc_stages[2] = 1.1;
    kc_stages[3] = 0.9;
  }

  if(Vcrop=="Sweet lupin")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 20;
    cropdevstage_days = 30;
    midseasonstage_days = 30;
    finalstage_days = 15;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.7;
    kc_stages[2] = 1.1;
    kc_stages[3] = 0.9;
  }

  if(Vcrop=="Lentil")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 20;
    cropdevstage_days = 35;
    midseasonstage_days = 65;
    finalstage_days = 45;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.7;
    kc_stages[2] = 1.1;
    kc_stages[3] = 0.9;
  }

  if(Vcrop=="Thick bean" | Vcrop=="Vetch" | Vcrop=="Other vegetables dry plant")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 15;
    cropdevstage_days = 30;
    midseasonstage_days = 35;
    finalstage_days = 20;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.7;
    kc_stages[2] = 1.1;
    kc_stages[3] = 0.9;
  }

  if(Vcrop=="Potatoes")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 20;
    cropdevstage_days = 30;
    midseasonstage_days = 35;
    finalstage_days = 25;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.45;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1.15;
    kc_stages[3] = 0.85;
  }

  if(Vcrop=="Sweet Potato")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 15;
    cropdevstage_days = 30;
    midseasonstage_days = 35;
    finalstage_days = 20;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.45;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1.15;
    kc_stages[3] = 0.85;
  }

  if(Vcrop=="Sugar Beet")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 25;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 40;
    }    
    initialstage_days = 25;
    cropdevstage_days = 35;
    midseasonstage_days = 65;
    finalstage_days = 40;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.45;
    kc_stages[1] = 0.8;
    kc_stages[2] = 1.15;
    kc_stages[3] = 0.8;
  }

  if(Vcrop=="Sugar Cane")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 50;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 20;
    cropdevstage_days = 30;
    midseasonstage_days = 60;
    finalstage_days = 40;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.45;
    kc_stages[1] = 0.8;
    kc_stages[2] = 1.15;
    kc_stages[3] = 0.8;
  }

  if(Vcrop=="Eggplant")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 30;
    cropdevstage_days = 40;
    midseasonstage_days = 45;
    finalstage_days = 25;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.45;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1.15;
    kc_stages[3] = 0.8;
  }
  
  if(Vcrop=="Garlic" | Vcrop=="Onion")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 25;
    cropdevstage_days = 40;
    midseasonstage_days = 20;
    finalstage_days = 10;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    if(Vcrop=="Garlic")
    {
      kc_stages[0] = 0.45;
      kc_stages[1] = 0.75;
      kc_stages[2] = 1;
      kc_stages[3] = 0.8; 
    }
    else
    {
      kc_stages[0] = 0.5;
      kc_stages[1] = 0.75;
      kc_stages[2] = 1;
      kc_stages[3] = 0.8;
    }
  }

  if(Vcrop=="Carrot")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 25;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 35;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 45;
    }    
    initialstage_days = 25;
    cropdevstage_days = 40;
    midseasonstage_days = 20;
    finalstage_days = 10;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.5;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1;
    kc_stages[3] = 0.8;
  }

  if(Vcrop=="Tomatoes")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 25;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 35;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 45;
    }    
    initialstage_days = 35;
    cropdevstage_days = 40;
    midseasonstage_days = 45;
    finalstage_days = 20;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.45;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1.15;
    kc_stages[3] = 0.8;
  }

  if(Vcrop=="Cabbage" | Vcrop=="Lettuce")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 25;
    cropdevstage_days = 40;
    midseasonstage_days = 20;
    finalstage_days = 10;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    if(Vcrop=="Cabbage")
    {
      kc_stages[0] = 0.5;
      kc_stages[1] = 0.75;
      kc_stages[2] = 1.05;
      kc_stages[3] = 0.75;
    }
    else
    {
      kc_stages[0] = 0.5;
      kc_stages[1] = 0.75;
      kc_stages[2] = 1;
      kc_stages[3] = 0.75;
    }
  }

  if(Vcrop=="Spinach")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 20;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 40;
    }    
    initialstage_days = 25;
    cropdevstage_days = 40;
    midseasonstage_days = 20;
    finalstage_days = 10;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.5;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1;
    kc_stages[3] = 0.75;
  }

  if(Vcrop=="Cucumber" | Vcrop=="Melons")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 50;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 60;
    }    
    initialstage_days = 25;
    cropdevstage_days = 40;
    midseasonstage_days = 20;
    finalstage_days = 10;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.5;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1.1;
    kc_stages[3] = 0.75;
  }

  if(Vcrop=="Tomatoes(Greenhouse)")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 15;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 20;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 30;
    }    
    initialstage_days = 20;
    cropdevstage_days = 35;
    midseasonstage_days = 35;
    finalstage_days = 15;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.5;
    kc_stages[1] = 0.8;
    kc_stages[2] = 1.15;
    kc_stages[3] = 0.8;
  }

  if(Vcrop=="Tomatoes(hydroponics)")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 15;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 20;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 30;
    }    
    initialstage_days = 15;
    cropdevstage_days = 30;
    midseasonstage_days = 30;
    finalstage_days = 10;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.5;
    kc_stages[1] = 0.65;
    kc_stages[2] = 1;
    kc_stages[3] = 0.65;
  }

//  if(Vcrop=="Cabbage" | Vcrop=="Lettuce")
//  {
//    if(Vsoil=="Shallow")
//    {
//      d_net_crop = 30;
//    }
//    if(Vsoil=="Loamy")
//    {
//      d_net_crop = 40;
//    }
//    if(Vsoil=="Clayey")
//    {
//      d_net_crop = 50;
//    }    
//    initialstage_days = 25;
//    cropdevstage_days = 40;
//    midseasonstage_days = 20;
//    finalstage_days = 10;
//    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
//    if(Vcrop=="Cabbage")
//    {
//      kc_stages[0] = 0.5;
//      kc_stages[1] = 0.75;
//      kc_stages[2] = 1.05;
//      kc_stages[3] = 0.75;
//    }
//    if(Vcrop=="Lettuce")
//    {
//      kc_stages[0] = 0.5;
//      kc_stages[1] = 0.75;
//      kc_stages[2] = 1;
//      kc_stages[3] = 0.75;
//    }
//  }

  if(Vcrop=="Forage(fodding) plants, pastures" | Vcrop=="Forage(fodding) plants-temporary pastures" 
  | Vcrop=="Other forage(fodding) plants, corn siloed" | Vcrop=="Other forage(fodding) plants"
  | Vcrop=="Seeds" | Vcrop=="Other plants")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 15;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 25;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 30;
    }    
    initialstage_days = 15;
    cropdevstage_days = 25;
    midseasonstage_days = 30;
    finalstage_days = 10;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    float kc_stages[4] = {0.35,  0.45,  0.8,  0.45};
    if(Vcrop=="Seeds" | Vcrop=="Other plants")
    {
      kc_stages[0] = 0.35;
      kc_stages[1] = 0.45;
      kc_stages[2] = 0.65;
      kc_stages[3] = 0.45;
    }
  }

  if(Vcrop=="Tobacco" | Vcrop=="Hop" | Vcrop=="Rape" | Vcrop=="Canola")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 25;
    cropdevstage_days = 30;
    midseasonstage_days = 65;
    finalstage_days = 40;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1.1;
    kc_stages[3] = 0.9;
  }

  if(Vcrop=="Turnip")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 55;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 65;
    }    
    initialstage_days = 25;
    cropdevstage_days = 30;
    midseasonstage_days = 65;
    finalstage_days = 40;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1.1;
    kc_stages[3] = 0.9;
  }

  if(Vcrop=="Sunflower" | Vcrop=="Soyabean" | Vcrop=="Oil" | Vcrop=="Oil crops(peanut)" | Vcrop=="Oilseed rape"
  | Vcrop=="Other oily plants" | Vcrop=="In fiber" | Vcrop=="Hemp fiber" | Vcrop=="Other textile plants")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 55;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 65;
    }    
    initialstage_days = 25;
    cropdevstage_days = 35;
    midseasonstage_days = 45;
    finalstage_days = 25;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    if(Vcrop=="Sunflower")
    {
      kc_stages[0] = 0.35;
      kc_stages[1] = 0.75;
      kc_stages[2] = 1.15;
      kc_stages[3] = 0.55;
    }
    if(Vcrop=="Soyabean" | Vcrop=="Oil")
    {
      kc_stages[0] = 0.35;
      kc_stages[1] = 0.75;
      kc_stages[2] = 1.1;
      kc_stages[3] = 0.55;
    }
    else
    {
      kc_stages[0] = 0.45;
      kc_stages[1] = 0.75;
      kc_stages[2] = 1.05;
      kc_stages[3] = 0.7;
    }
  }

  if(Vcrop=="Aromatic & medicinal plants")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 60;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 15;
    cropdevstage_days = 25;
    midseasonstage_days = 35;
    finalstage_days = 15;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.65;
    kc_stages[2] = 1;
    kc_stages[3] = 0.65;
  }

  if(Vcrop=="Other industrial plants" | Vcrop=="Family gardens(vegees)")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 15;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 20;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 30;
    }    
    initialstage_days = 15;
    cropdevstage_days = 25;
    midseasonstage_days = 35;
    finalstage_days = 15;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.65;
    kc_stages[2] = 1;
    kc_stages[3] = 0.65;
  }

  if(Vcrop=="Permanent pastures meadows" | Vcrop=="Pastures hayfields perm. rough fields")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 15;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 20;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 30;
    }    
    initialstage_days = 10;
    cropdevstage_days = 15;
    midseasonstage_days = 25;
    finalstage_days = 15;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.45;
    kc_stages[2] = 0.65;
    kc_stages[3] = 0.45;
  }

  // problem here soft wdt reset
  if(Vcrop=="Apple" | Vcrop=="Plum" | Vcrop=="Pears" | Vcrop=="Peach" | Vcrop=="Cherry" | Vcrop=="Apricot")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 60;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 30;
    cropdevstage_days = 60;
    midseasonstage_days = 90;
    finalstage_days = 30;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.45;
    kc_stages[2] = 0.95;
    kc_stages[3] = 0.75;
  }

  if(Vcrop=="Walnut" | Vcrop=="Hazelnut" | Vcrop=="Almond" | Vcrop=="Celery")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 60;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 20;
    cropdevstage_days = 30;
    midseasonstage_days = 120;
    finalstage_days = 30;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.5;
    kc_stages[2] = 1.1;
    kc_stages[3] = 0.65;
  }

  if(Vcrop=="Cotton")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 60;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 30;
    cropdevstage_days = 50;
    midseasonstage_days = 65;
    finalstage_days = 45;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.45;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1.15;
    kc_stages[3] = 0.75;
  }

  if(Vcrop=="Cassava(TropicalPot)")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 60;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 60;
    cropdevstage_days = 90;
    midseasonstage_days = 120;
    finalstage_days = 60;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.45;
    kc_stages[1] = 0.75;
    kc_stages[2] = 1.15;
    kc_stages[3] = 0.75;
  }

  if(Vcrop=="Orange")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 60;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 20;
    cropdevstage_days = 70;
    midseasonstage_days = 90;
    finalstage_days = 30;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.7;
    kc_stages[2] = 0.6;
    kc_stages[3] = 0.7;
  }

  if(Vcrop=="Currant" | Vcrop=="Fig" | Vcrop=="Raspberry")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 50;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 30;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 50;
    }    
    initialstage_days = 20;
    cropdevstage_days = 30;
    midseasonstage_days = 60;
    finalstage_days = 30;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.45;
    kc_stages[2] = 0.75;
    kc_stages[3] = 0.45;
  }

  if(Vcrop=="Noble wine grape" | Vcrop=="Other wine-grape" | Vcrop=="Grape wine-table wine")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 40;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 55;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 70;
    }    
    initialstage_days = 20;
    cropdevstage_days = 40;
    midseasonstage_days = 80;
    finalstage_days = 30;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    if(Vcrop=="Noble wine grape")
    {
      kc_stages[0] = 0.35;
      kc_stages[1] = 0.45;
      kc_stages[2] = 0.75;
      kc_stages[3] = 0.65;
    }
    if(Vcrop=="Other wine grape")
    {
      kc_stages[0] = 0.35;
      kc_stages[1] = 0.45;
      kc_stages[2] = 0.8;
      kc_stages[3] = 0.65;
    }
    if(Vcrop=="Grape wine-table wine")
    {
      kc_stages[0] = 0.35;
      kc_stages[1] = 0.45;
      kc_stages[2] = 0.8;
      kc_stages[3] = 0.45;
    }
  }

  if(Vcrop=="Grain legumes")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 25;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 35;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 45;
    }    
    initialstage_days = 20;
    cropdevstage_days = 40;
    midseasonstage_days = 80;
    finalstage_days = 30;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.35;
    kc_stages[1] = 0.45;
    kc_stages[2] = 0.75;
    kc_stages[3] = 0.45;
  }

  if(Vcrop=="Mushrooms")
  {
    if(Vsoil=="Shallow")
    {
      d_net_crop = 10;
    }
    if(Vsoil=="Loamy")
    {
      d_net_crop = 15;
    }
    if(Vsoil=="Clayey")
    {
      d_net_crop = 20;
    }    
    initialstage_days = 10;
    cropdevstage_days = 20;
    midseasonstage_days = 30;
    finalstage_days = 20;
    growing_days = initialstage_days+cropdevstage_days+midseasonstage_days+finalstage_days;
    kc_stages[0] = 0.3;
    kc_stages[1] = 0.45;
    kc_stages[2] = 0.65;
    kc_stages[3] = 0.45;
  }

  int stages[4] = {initialstage_days, cropdevstage_days, midseasonstage_days, finalstage_days};
  Serial.print("\nCrop selected: ");
  Serial.println(Vcrop);
  Serial.print("DOP: ");
  Serial.print(Vdate);
  Serial.print("/");
  Serial.print(Vmonth);
  Serial.print("/");
  Serial.println(Vyear);
  Serial.print("Soil type: ");
  Serial.println(Vsoil);
  Serial.print("kc: ");
  Serial.print(kc_stages[0]);
  Serial.print(" ");
  Serial.print(kc_stages[1]);
  Serial.print(" ");
  Serial.print(kc_stages[2]);
  Serial.print(" ");
  Serial.println(kc_stages[3]);
  Serial.print("Net irrigation depth (cm): ");
  Serial.println(d_net_crop);
  Serial.print("Initial Stage days: ");
  Serial.println(initialstage_days);
  Serial.print("Crop dev Stage days: ");
  Serial.println(cropdevstage_days);
  Serial.print("Mid season Stage days: ");
  Serial.println(midseasonstage_days);
  Serial.print("Final Stage days: ");
  Serial.println(finalstage_days);
  Serial.println();

  //setting the dates
  Vdate_i = Vdate.toInt();
  Vmonth_i = Vmonth.toInt();
  Vyear_i = Vyear.toInt();
  DateTime ini(Vyear_i,Vmonth_i,Vdate_i,0,0,0);
  mid = ini + TimeSpan(initialstage_days,0,0,0);
  dev = mid + TimeSpan(cropdevstage_days,0,0,0);
  fin = dev + TimeSpan(midseasonstage_days,0,0,0);
  fin_end = fin + TimeSpan(finalstage_days,0,0,0);
  showDate(ini, "Initial: ");
  showDate(mid, "Mid season: ");
  showDate(dev, "Crop dev: ");
  showDate(fin, "Final: ");
  showDate(fin_end, "End: ");
  Serial.println();

  //setting the available months and days
  int j=ini.month()-1;
//  while(1)
  for(int trial=0;trial<12;trial++)
  {
    if(month_loc[j]==ini.month())
    {
      days_avail[j] = days_in_months[j]-ini.day();
    }
    if(month_loc[j]==fin_end.month())
    {
      days_avail[j] = fin_end.day();
    }
    if(month_loc[j]!=ini.month() & month_loc[j]!=fin_end.month())
    {
      days_avail[j] = days_in_months[j];
    }
    j++;
    j=j%12;
    if(j==fin_end.month())
    {
      break;
    }
  }

  for(int i=0;i<12;i++)
  {
    Serial.print(name_months[i]);
    Serial.print(":\t");
    Serial.println(days_avail[i]);
  }
  Serial.println();

  // setting days
  j=ini.month()-1;
  int i=0, avail_days=days_avail[j], x=0;
  while(1)
  {
    x = avail_days-stages[i]+x;
    if(x>=0)
    {
      days_dist[i][j] = avail_days-x;
      i++;
      days_dist[i][j] = x;
      j++;
      j=j%12;
      avail_days = days_avail[j];
    }
    if(x<0)
    {
      while(x<0)
      {
        days_dist[i][j] = avail_days;
        j++;
        j=j%12;
        x = days_avail[j] + x;
        avail_days = days_avail[j];
      }
      days_dist[i][j] = days_avail[j]-x;
      avail_days = x;
      x=0;
      i++;
    }
    if(i>3)
    {
      break;
    }    
  }

  // printing the days available stagewise
  Serial.println("Days distribution:");
  Serial.println("J F M A M J J A S O N D");
  for(int i=0;i<4;i++)
  {
    for(int j=0;j<12;j++)
    {
      Serial.print(days_dist[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }

  Serial.println("\nkc:");
  float sum_kc;
  int sum_days;
  for(int j=0;j<12;j++)
  {
    sum_kc = 0;
    sum_days = 0;
    for(int i=0;i<4;i++)
    {
      sum_kc = sum_kc + kc_stages[i]*days_dist[i][j];
      sum_days = sum_days + days_dist[i][j];
    }
    if(sum_days>0)
    {
      kc[j] = sum_kc/sum_days;
    }
    Serial.print(name_months[j]);
    Serial.print("\t");
    Serial.println(kc[j]);
  }
}
//-----------------------------------------------------


