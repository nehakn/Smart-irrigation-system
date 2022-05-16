//
//  HTML PAGE
//
const char PAGE_Trial[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Automatic irrigation application</strong>
<hr>
<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" style="width:350px" >
<tr><td>Crop Name:</td><td>
<select  id="cropName" name="cropName">
  <option> … </option>
  <option> Normal Wheat  </option>
  <option> Rough Wheat </option>
  <option> Rye   </option>
  <option> Barley  </option>
  <option> Oat </option>
  <option> Corn  </option>
  <option> Maize </option>
  <option> Rice  </option>
  <option> Peas  </option>
  <option> Beans </option>
  <option> Sweet lupin </option>
  <option> Lentil  </option>
  <option> Thick bean  </option>
  <option> Vetch </option>
  <option> Other vegetables dry plant  </option>
  <option> Potatoes  </option>
  <option> Sweet Potato  </option>
  <option> Sugar Beet  </option>
  <option> Sugar Cane  </option>
  <option> Eggplant  </option>
  <option> Garlic  </option>
  <option> Onion </option>
  <option> Carrot  </option>
  <option> Tomatoes  </option>
  <option> Cabbage </option>
  <option> Lettuce </option>
  <option> Spinach </option>
  <option> Cucumber  </option>
  <option> Melons  </option>
  <option> Tomatoes(Greenhouse)  </option>
  <option> Tomatoes(hydroponics) </option>
  <option> Forage(fodding) plants, pastures </option>
  <option> Forage(fodding) plants-temporary pastures </option>
  <option> Other forage(fodding) plants, corn siloed  </option>
  <option> Other forage(fodding) plants </option>
  <option> Seeds </option>
  <option> Other plants  </option>
  <option> Tobacco </option>
  <option> Hop </option>
  <option> Rape  </option>
  <option> Canola  </option>
  <option> Turnip  </option>
  <option> Sunflower </option>
  <option> Soybean </option>
  <option> Oil  </option>
  <option> Oil crops(peanut) </option>
  <option> Oilseed rape  </option>
  <option> Other oily plants </option>
  <option> In fiber  </option>
  <option> Hemp fiber  </option>
  <option> Other textile plants  </option>
  <option> Aromatic & medicinal plants </option>
  <option> Other industrial plants </option>
  <option> Family gardens(vegees)  </option>
  <option> Permanent pastures meadows  </option>
  <option> Pastures hayfields perm. rough fields </option>
  <option> Apple </option>
  <option> Plum  </option>
  <option> Pears </option>
  <option> Peach </option>
  <option> Cherry  </option>
  <option> Apricot </option>
  <option> Walnut </option>
  <option> Hazelnut </option>
  <option> Almond  </option>
  <option> Celery  </option>
  <option> Cotton  </option>
  <option> Cassava(TropicalPot)  </option>
  <option> Orange  </option>
  <option> Currant </option>
  <option> Fig </option>
  <option> Raspberry </option>
  <option> Noble wine grape  </option>
  <option> Other wine-grape  </option>
  <option> Grape wine-table wine  </option>
  <option> Grain legumes </option>
  <option> Mushrooms </option>
</select>
</td></tr>
<tr><td>Date of plantation:</td><td>
<select  id="dateOfPlantationdd" name="DD">
  <option>  1 </option>
  <option>  2 </option>
  <option>  3 </option>
  <option>  4 </option>
  <option>  5 </option>
  <option>  6 </option>
  <option>  7 </option>
  <option>  8 </option>
  <option>  9 </option>
  <option>  10  </option>
  <option>  11  </option>
  <option>  12  </option>
  <option>  13  </option>
  <option>  14  </option>
  <option>  15  </option>
  <option>  16  </option>
  <option>  17  </option>
  <option>  18  </option>
  <option>  19  </option>
  <option>  20  </option>
  <option>  21  </option>
  <option>  22  </option>
  <option>  23  </option>
  <option>  24  </option>
  <option>  25  </option>
  <option>  26  </option>
  <option>  27  </option>
  <option>  28  </option>
  <option>  29  </option>
  <option>  30  </option>
  <option>  31  </option>
</select>
/<select id="dateOfPlantationmm" name="MM">
  <option>  1 </option>
  <option>  2 </option>
  <option>  3 </option>
  <option>  4 </option>
  <option>  5 </option>
  <option>  6 </option>
  <option>  7 </option>
  <option>  8 </option>
  <option>  9 </option>
  <option>  10  </option>
  <option>  11  </option>
  <option>  12  </option>
</select> 
<select id="dateOfPlantationyy" name="YYYY">
  <option>  2018  </option>
  <option>  2019  </option>
  <option>  2020  </option>
  <option>  2021  </option>
  <option>  2022  </option>
  <option>  2023  </option>
  <option>  2024  </option>
  <option>  2025  </option>
  <option>  2026  </option>
  <option>  2027  </option>
  <option>  2028  </option>
</select>  
</td></tr>
<tr><td>Soil type:</td><td>
<select id="soilType" name="soilType">
  <option> Shallow </option>
  <option> Loamy </option>
  <option> Clayey </option>
</select>
</td></tr>
<tr><td>Location latitude:</td><td>
<input type="text" id="latitude" name="latitude">
</td></tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>

<script>

function GetState()
{
  setValues("/admin/cropSel");
}


window.onload = function ()
{
  load("style.css","css", function() 
  {
    load("microajax.js","js", function() 
    {
          setValues("/admin/cropSel");
    });
  });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
</script>
)=====";

//
//  SEND HTML PAGE OR IF A FORM SUMBITTED VALUES, PROCESS THESE VALUES
// 
void send_trial_html()
{
  if (server.args() > 0 )  // Save Settings
  {
    for ( uint8_t i = 0; i < server.args(); i++ ) 
    {
      if (server.argName(i) == "cropName") config.cropName =   urldecode(server.arg(i));
      if (server.argName(i) == "DD") config.dateOfPlantation[0] =  urldecode(server.arg(i)).toInt();
      if (server.argName(i) == "MM") config.dateOfPlantation[1] =  urldecode(server.arg(i)).toInt();
      if (server.argName(i) == "YYYY") config.dateOfPlantation[2] =  urldecode(server.arg(i)).toInt();
//      if (server.argName(i) == "cdDD") config.currentDate[0] =  urldecode(server.arg(i)).toInt();
//      if (server.argName(i) == "cdMM") config.currentDate[1] =  urldecode(server.arg(i)).toInt();
//      if (server.argName(i) == "cdYYYY") config.currentDate[2] =  urldecode(server.arg(i)).toInt();
      if (server.argName(i) == "soilType") config.soilType =    urldecode(server.arg(i));
      if (server.argName(i) == "latitude") config.latitude =  urldecode(server.arg(i)).toFloat();
    }    
    WriteConfig();
  }
   server.send ( 200, "text/html", PAGE_Trial ); 
  Serial.println(__FUNCTION__); 
}

//
//   FILL THE PAGE WITH VALUES
//
void send_trial_values_html()
{
  String values ="";
  values += "cropName|" + (String) config.cropName + "|input\n";
  values += "dateOfPlantationdd|" +  (String) config.dateOfPlantation[0] + "|input\n";
  values += "dateOfPlantationmm|" +  (String) config.dateOfPlantation[1] + "|input\n";
  values += "dateOfPlantationyy|" +  (String) config.dateOfPlantation[2] + "|input\n";
//  values += "currentdatedd|" +  (String) config.currentDate[0] + "|input\n";
//  values += "currentdatemm|" +  (String) config.currentDate[1] + "|input\n";
//  values += "currentdateyy|" +  (String) config.currentDate[2] + "|input\n";
  values += "soilType|" +  (String) config.soilType + "|input\n";
  values += "latitude|" +  (String) config.latitude + "|input\n";
  
  server.send ( 200, "text/plain", values);
  Serial.print("1 ");
  Serial.println(__FUNCTION__); 
  AdminTimeOutCounter=0;
}
