#pragma once

#include <Mesh.h>
#include <helpers/SensorManager.h>
#include <helpers/sensors/LocationProvider.h>

// Forward declaration
class BaseChatMesh;

class EnvironmentSensorManager : public SensorManager {
protected:
  int next_available_channel = TELEM_CHANNEL_SELF + 1;

  bool AHTX0_initialized = false;
  bool BME280_initialized = false;
  bool BMP280_initialized = false;
  bool INA3221_initialized = false;
  bool INA219_initialized = false;

  bool gps_detected = false;
  bool gps_active = false;

  #if ENV_INCLUDE_GPS
  LocationProvider* _location;
  BaseChatMesh* _mesh = nullptr;  // For location sharing callback
  double last_reported_lat = 0.0;
  double last_reported_lon = 0.0;
  void start_gps();
  void stop_gps();
  void initBasicGPS();
  #endif


public:
  #if ENV_INCLUDE_GPS
  EnvironmentSensorManager(LocationProvider &location): _location(&location){};
  void setMeshForLocationSharing(BaseChatMesh* mesh) { _mesh = mesh; }
  #else
  EnvironmentSensorManager(){};
  #endif
  bool begin() override;
  bool querySensors(uint8_t requester_permissions, CayenneLPP& telemetry) override;
  #if ENV_INCLUDE_GPS
  void loop() override;
  #endif
  int getNumSettings() const override;
  const char* getSettingName(int i) const override;
  const char* getSettingValue(int i) const override;
  bool setSettingValue(const char* name, const char* value) override;
};
