#pragma once

#include <CayenneLPP.h>

#define TELEM_PERM_BASE         0x01   // 'base' permission includes battery
#define TELEM_PERM_LOCATION     0x02
#define TELEM_PERM_ENVIRONMENT  0x04   // permission to access environment sensors

#define TELEM_CHANNEL_SELF   1   // LPP data channel for 'self' device

class BaseChatMesh;  // Forward declaration

class SensorManager {
public:
  double node_lat, node_lon;  // modify these, if you want to affect Advert location
  double node_altitude;       // altitude in meters

protected:
  BaseChatMesh* _mesh = nullptr;  // For location marker functionality
  double last_lat = 0.0;
  double last_lon = 0.0;
  bool location_marker_enabled = true;
  bool first_gps_fix_received = false;  // Track if we've ever had a valid GPS fix

public:
  SensorManager() { node_lat = 0; node_lon = 0; node_altitude = 0; }
  virtual bool begin() { return false; }
  virtual bool querySensors(uint8_t requester_permissions, CayenneLPP& telemetry) { return false; }
  virtual void loop() { }
  virtual int getNumSettings() const { return 0; }
  virtual const char* getSettingName(int i) const { return NULL; }
  virtual const char* getSettingValue(int i) const { return NULL; }
  virtual bool setSettingValue(const char* name, const char* value) { return false; }
  virtual void setMesh(BaseChatMesh* mesh) { _mesh = mesh; }  // Override in sensor managers that support location sharing
  
  // Location marker functionality
  void updateLocationMarker();
  void setLocationMarkerEnabled(bool enabled) { location_marker_enabled = enabled; }
  
  // GPS update functionality - should be called by target-specific managers when GPS coordinates are updated
  void onGPSUpdate();
};
