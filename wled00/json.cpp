#include "wled.h"
#include "lighted_objects/LightedObjectFactory.h"
#include "lighted_objects/ILightedObject.h"

#include <string>

/*
 * JSON API (De)serialization
 */

bool deserializeState(JsonObject root)
{
#ifdef ENABLE_SET_EFFECTS // MDR TEMP - removing set effects functionality
  strip.applyToAllSelected = false;
#endif // ENABLE_SET_EFFECTS
  bool stateResponse = root[F("v")] | false;
  
  bri = root["bri"] | bri;
  lightDisplay.setBrightness(bri);

  bool on = root["on"] | (bri > 0);
  if (!on != !bri) toggleOnOff();

  int tr = root[F("transition")] | -1;
  if (tr >= 0)
  {
    transitionDelay = tr;
    transitionDelay *= 100;
  }

  tr = root[F("tt")] | -1;
  if (tr >= 0)
  {
    transitionDelayTemp = tr;
    transitionDelayTemp *= 100;
    jsonTransitionOnce = true;
  }
  
  int cy = root[F("pl")] | -2;
  if (cy > -2) presetCyclingEnabled = (cy >= 0);
  JsonObject ccnf = root["ccnf"];
  presetCycleMin = ccnf[F("min")] | presetCycleMin;
  presetCycleMax = ccnf[F("max")] | presetCycleMax;
  tr = ccnf[F("time")] | -1;
  if (tr >= 2) presetCycleTime = tr;

  JsonObject nl = root["nl"];
  nightlightActive    = nl["on"]      | nightlightActive;
  nightlightDelayMins = nl[F("dur")]  | nightlightDelayMins;
  nightlightMode      = nl[F("fade")] | nightlightMode; //deprecated, remove for v0.12.0
  nightlightMode      = nl[F("mode")] | nightlightMode;
  nightlightTargetBri = nl[F("tbri")] | nightlightTargetBri;

  JsonObject udpn = root["udpn"];
  notifyDirect         = udpn[F("send")] | notifyDirect;
  receiveNotifications = udpn[F("recv")] | receiveNotifications;
  bool noNotification  = udpn[F("nn")]; //send no notification just for this request

  unsigned long timein = root[F("time")] | -1;
  if (timein != -1) {
    if (millis() - ntpLastSyncTime > 50000000L) setTime(timein);
    if (presetsModifiedTime == 0) presetsModifiedTime = timein;
  }

  doReboot = root[F("rb")] | doReboot;

  realtimeOverride = root[F("lor")] | realtimeOverride;
  if (realtimeOverride > 2) realtimeOverride = REALTIME_OVERRIDE_ALWAYS;

  if (root.containsKey("live")) {
    bool lv = root["live"];
    if (lv) realtimeLock(65000); //enter realtime without timeout
    else    realtimeTimeout = 0; //cancel realtime mode immediately
  }

  // Handle object actions
  if (root.containsKey("object_action"))
  {
    JsonObject objectAction = root["object_action"];
    String actionType = objectAction["action"];

    if (actionType.compareTo("clear_all_objects") == 0)
    {
      lightDisplay.clearAllObjects();
    }
    else if (actionType.compareTo("create_object") == 0)
    {
      String objectType = objectAction["object_type"];
      lightDisplay.createLightedObject(objectType.c_str());
    }
    else if (actionType.compareTo("delete") == 0)
    {
      int objectIndex = objectAction[F("object_index")] | -1;
      lightDisplay.deleteObject(objectIndex);
    }
    else if (actionType.compareTo("move_down") == 0)
    {
      int objectIndex = objectAction[F("object_index")] | -1;
      lightDisplay.moveObjectDown(objectIndex);
    }
    else if (actionType.compareTo("move_up") == 0)
    {
      int objectIndex = objectAction[F("object_index")] | -1;
      lightDisplay.moveObjectUp(objectIndex);
    }
    else if (actionType.compareTo("save_changes") == 0)
    {
      int objectIndex = objectAction[F("object_index")] | -1;
      String userInputValues = objectAction["userInputs"];
      lightDisplay.updateObject(objectIndex, userInputValues.c_str());
    }
    else if (actionType.compareTo("toggle_power") == 0)
    {
      int objectIndex = objectAction[F("object_index")] | -1;
      lightDisplay.togglePower(objectIndex);
    }
  }

  usermods.readFromJsonState(root);

  int ps = root[F("psave")] | -1;
  if (ps > 0) {
    savePreset(ps, true, nullptr, root);
  } else {
    ps = root[F("pdel")] | -1; //deletion
    if (ps > 0) {
      deletePreset(ps);
    }
    ps = root["ps"] | -1; //load preset (clears state request!)
    if (ps >= 0) {applyPreset(ps); return stateResponse;}

    //HTTP API commands
    const char* httpwin = root["win"];
    if (httpwin) {
      String apireq = "win&";
      apireq += httpwin;
      handleSet(nullptr, apireq, false);
    }
  }

  JsonObject playlist = root[F("playlist")];
  if (!playlist.isNull()) {
    loadPlaylist(playlist); return stateResponse;
  }

  colorUpdated(noNotification ? NOTIFIER_CALL_MODE_NO_NOTIFY : NOTIFIER_CALL_MODE_DIRECT_CHANGE);

  return stateResponse;
}

void serializeState(JsonObject root, bool forPreset, bool includeBri, bool segmentBounds)
{ 
  if (includeBri) 
  {
    root["on"] = (bri > 0);
    root["bri"] = lightDisplay.getBrightness();
    root[F("transition")] = transitionDelay/100; //in 100ms
  }

  if (!forPreset)
  {
    if (errorFlag)
    {
      root[F("error")] = errorFlag;
    }
  }
    
  root[F("ps")] = currentPreset;
  root[F("pss")] = savedPresets;
  root[F("pl")] = (presetCyclingEnabled) ? 0: -1;
    
  usermods.addToJsonState(root);

  //temporary for preset cycle
  JsonObject ccnf = root.createNestedObject("ccnf");
  ccnf[F("min")] = presetCycleMin;
  ccnf[F("max")] = presetCycleMax;
  ccnf[F("time")] = presetCycleTime;

  JsonObject nl = root.createNestedObject("nl");
  nl["on"] = nightlightActive;
  nl[F("dur")] = nightlightDelayMins;
  nl[F("fade")] = (nightlightMode > NL_MODE_SET); //deprecated
  nl[F("mode")] = nightlightMode;
  nl[F("tbri")] = nightlightTargetBri;
  if (nightlightActive) 
  {
    nl[F("rem")] = (nightlightDelayMs - (millis() - nightlightStartTime)) / 1000; // seconds remaining
  } 
  else 
  {
    nl[F("rem")] = -1;
  }

  JsonObject udpn = root.createNestedObject("udpn");
  udpn[F("send")] = notifyDirect;
  udpn[F("recv")] = receiveNotifications;

  root[F("lor")] = realtimeOverride;
}

//by https://github.com/tzapu/WiFiManager/blob/master/WiFiManager.cpp
int getSignalQuality(int rssi)
{
    int quality = 0;

    if (rssi <= -100)
    {
        quality = 0;
    }
    else if (rssi >= -50)
    {
        quality = 100;
    }
    else
    {
        quality = 2 * (rssi + 100);
    }
    return quality;
}

void serializeInfo(JsonObject root)
{
  root[F("ver")] = versionString;
  root[F("vid")] = VERSION;
  //root[F("cn")] = WLED_CODENAME;
  
  JsonObject leds = root.createNestedObject("leds");
  leds[F("count")] = ledCount;
  leds[F("rgbw")] = useRGBW;
//  leds[F("wv")] = useRGBW && (strip.rgbwMode == RGBW_MODE_MANUAL_ONLY || strip.rgbwMode == RGBW_MODE_DUAL); //should a white channel slider be displayed?
  leds[F("wv")] = useRGBW && (lightDisplay.getRgbwMode() == RGBW_MODE_MANUAL_ONLY || lightDisplay.getRgbwMode() == RGBW_MODE_DUAL); //should a white channel slider be displayed?
  JsonArray leds_pin = leds.createNestedArray("pin");
  leds_pin.add(LEDPIN);
  
  leds[F("pwr")] = lightDisplay.getCurrentMilliamps();
  leds[F("maxpwr")] = lightDisplay.getCurrentMilliamps() ? lightDisplay.getMaximumAllowedCurrent() : 0;
  leds[F("maxseg")] = lightDisplay.getNumberOfLightedObjects();
  leds[F("seglock")] = false; //will be used in the future to prevent modifications to segment config

  root[F("str")] = syncToggleReceive;
  
  root[F("name")] = serverDescription;
  root[F("udpport")] = udpPort;
  root["live"] = (bool)realtimeMode;

  switch (realtimeMode) {
    case REALTIME_MODE_INACTIVE: root["lm"] = ""; break;
    case REALTIME_MODE_GENERIC:  root["lm"] = ""; break;
    case REALTIME_MODE_UDP:      root["lm"] = F("UDP"); break;
    case REALTIME_MODE_HYPERION: root["lm"] = F("Hyperion"); break;
    case REALTIME_MODE_E131:     root["lm"] = F("E1.31"); break;
    case REALTIME_MODE_ADALIGHT: root["lm"] = F("USB Adalight/TPM2"); break;
    case REALTIME_MODE_ARTNET:   root["lm"] = F("Art-Net"); break;
    case REALTIME_MODE_TPM2NET:  root["lm"] = F("tpm2.net"); break;
    case REALTIME_MODE_DDP:      root["lm"] = F("DDP"); break;
  }

  if (realtimeIP[0] == 0)
  {
    root[F("lip")] = "";
  } else {
    root[F("lip")] = realtimeIP.toString();
  }

  #ifdef WLED_ENABLE_WEBSOCKETS
  root[F("ws")] = ws.count();
  #else
  root[F("ws")] = -1;
  #endif

  JsonObject wifi_info = root.createNestedObject("wifi");
  wifi_info[F("bssid")] = WiFi.BSSIDstr();
  int qrssi = WiFi.RSSI();
  wifi_info[F("rssi")] = qrssi;
  wifi_info[F("signal")] = getSignalQuality(qrssi);
  wifi_info[F("channel")] = WiFi.channel();

  JsonObject fs_info = root.createNestedObject("fs");
  fs_info["u"] = fsBytesUsed / 1000;
  fs_info["t"] = fsBytesTotal / 1000;
  fs_info[F("pmt")] = presetsModifiedTime;
  
  #ifdef ARDUINO_ARCH_ESP32
  #ifdef WLED_DEBUG
    wifi_info[F("txPower")] = (int) WiFi.getTxPower();
    wifi_info[F("sleep")] = (bool) WiFi.getSleep();
  #endif
  root[F("arch")] = "esp32";
  root[F("core")] = ESP.getSdkVersion();
  //root[F("maxalloc")] = ESP.getMaxAllocHeap();
  #ifdef WLED_DEBUG
    root[F("resetReason0")] = (int)rtc_get_reset_reason(0);
    root[F("resetReason1")] = (int)rtc_get_reset_reason(1);
  #endif
  root[F("lwip")] = 0;
  #else
  root[F("arch")] = "esp8266";
  root[F("core")] = ESP.getCoreVersion();
  //root[F("maxalloc")] = ESP.getMaxFreeBlockSize();
  #ifdef WLED_DEBUG
    root[F("resetReason")] = (int)ESP.getResetInfoPtr()->reason;
  #endif
  root[F("lwip")] = LWIP_VERSION_MAJOR;
  #endif
  
  root[F("freeheap")] = ESP.getFreeHeap();
  root[F("uptime")] = millis()/1000 + rolloverMillis*4294967;

  
  usermods.addToJsonInfo(root);
  
  byte os = 0;
  #ifdef WLED_DEBUG
  os  = 0x80;
  #endif
  #ifndef WLED_DISABLE_ALEXA
  os += 0x40;
  #endif
  #ifndef WLED_DISABLE_BLYNK
  os += 0x20;
  #endif
  #ifndef WLED_DISABLE_CRONIXIE
  os += 0x10;
  #endif
  #ifndef WLED_DISABLE_FILESYSTEM
  os += 0x08;
  #endif
  #ifndef WLED_DISABLE_HUESYNC
  os += 0x04;
  #endif
  #ifdef WLED_ENABLE_ADALIGHT
  os += 0x02;
  #endif
  #ifndef WLED_DISABLE_OTA 
  os += 0x01;
  #endif
  root[F("opt")] = os;
  
  root[F("brand")] = "WLED";
  root[F("product")] = F("FOSS");
  root["mac"] = escapedMac;
}

void serveJson(AsyncWebServerRequest* request)
{
  byte subJson = 0;
  const String& url = request->url();
  if      (url.indexOf("state") > 0) subJson = 1;
  else if (url.indexOf("info")  > 0) subJson = 2;
  else if (url.indexOf("si") > 0) subJson = 3;
  else if (url.indexOf("live")  > 0) {
    serveLiveLeds(request);
    return;
  }
  else if (url.indexOf(F("eff"))   > 0) {
    request->send_P(200, "application/json", JSON_mode_names);
    return;
  }
  else if (url.indexOf(F("pal"))   > 0) {
    request->send_P(200, "application/json", JSON_palette_names);
    return;
  }
  else if (url.indexOf(F("colorset")) > 0) {
    request->send_P(200, "application/json", JSON_colorset_names);
    return;
  }
  else if (url.length() > 6) { //not just /json
    request->send(  501, "application/json", F("{\"error\":\"Not implemented\"}"));
    return;
  }
  
  AsyncJsonResponse* response = new AsyncJsonResponse(JSON_BUFFER_SIZE);
  JsonObject doc = response->getRoot();

  switch (subJson)
  {
    case 1: //state
      serializeState(doc); break;
    case 2: //info
      serializeInfo(doc); break;
    default: //all
      JsonObject state = doc.createNestedObject("state");
      serializeState(state);
      JsonObject info  = doc.createNestedObject("info");
      serializeInfo(info);
      if (subJson != 3)
      {
        doc[F("effects")]  = serialized((const __FlashStringHelper*)JSON_mode_names);
        doc[F("palettes")] = serialized((const __FlashStringHelper*)JSON_palette_names);
        doc[F("colorsets")] = serialized((const __FlashStringHelper*)JSON_colorset_names);

        StringList supportedObjects = LightedObjectFactory::get().getListOfLightedObjectTypes();
        JsonArray lightedObjects = doc.createNestedArray("supportedObjectTypes");
        for (std::string objectType : supportedObjects)
        {
          lightedObjects.add(objectType.c_str());
        }
      }
  }

  // MDR DEBUG - TODO - Create a LightDisplaySettings object to store these values and pass a json object to serialize deserialize them
  JsonObject lightedDisplayObject = doc.createNestedObject("light_display");
  lightedDisplayObject[F("led_count")] = lightDisplay.getNumberOfLEDs();
  lightedDisplayObject[F("rgbw_mode")] = lightDisplay.getSupportsWhiteChannel();
  lightedDisplayObject[F("use_white_channel")] = lightDisplay.useWhiteChannel();

  JsonObject powerStats = lightedDisplayObject.createNestedObject("power_stats");
  powerStats[F("current")] = 13;
  powerStats[F("max_current")] = lightDisplay.getMaximumAllowedCurrent();

  JsonArray ledPinsArray = lightedDisplayObject.createNestedArray("led_pins");
  ledPinsArray.add(LEDPIN);

  JsonArray lightedObjectArray = lightedDisplayObject.createNestedArray("lighted_objects");
  for (ILightedObject* lightedObject : lightDisplay.getLightedObjects())
  {
    JsonObject currentLightedObject = lightedObjectArray.createNestedObject();
    lightedObject->serializeCurrentStateToJson(currentLightedObject);
  }
  
  //Serial.printf("-----------------------------------------------------------------\nMDR DEBUG - Sending JSON Response:\n");
  //serializeJsonPretty(doc, Serial);
  //Serial.printf("-----------------------------------------------------------------\n");

  response->setLength();
  request->send(response);
}

#define MAX_LIVE_LEDS 180

bool serveLiveLeds(AsyncWebServerRequest* request, uint32_t wsClient)
{
  AsyncWebSocketClient * wsc;
  if (!request) { //not HTTP, use Websockets
    #ifdef WLED_ENABLE_WEBSOCKETS
    wsc = ws.client(wsClient);
    if (!wsc || wsc->queueLength() > 0) return false; //only send if queue free
    #endif
  }

  uint16_t used = ledCount;
  uint16_t n = (used -1) /MAX_LIVE_LEDS +1; //only serve every n'th LED if count over MAX_LIVE_LEDS
  char buffer[2000];
  strcpy_P(buffer, PSTR("{\"leds\":["));
  obuf = buffer;
  olen = 9;

  for (uint16_t i= 0; i < used; i += n)
  {
    olen += sprintf(obuf + olen, "\"%06X\",", lightDisplay.getPixelColor(i));
  }
  olen -= 1;
  oappend((const char*)F("],\"n\":"));
  oappendi(n);
  oappend("}");
  if (request) {
    request->send(200, "application/json", buffer);
  }
  #ifdef WLED_ENABLE_WEBSOCKETS
  else {
    wsc->text(obuf, olen);
  }
  #endif
  return true;
}
