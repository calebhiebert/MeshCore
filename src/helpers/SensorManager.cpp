#include "SensorManager.h"
#include "BaseChatMesh.h"
#include <cmath>

void SensorManager::updateLocationMarker() {
  if (!_mesh || !location_marker_enabled) {
    return;
  }
  
  // Only update location marker when location changes significantly
  // This prevents excessive fake advertisements on minor GPS variations
  if (node_lat != 0.0 || node_lon != 0.0) {
    
    // Check if location has changed significantly
    double lat_diff = fabs(node_lat - last_lat);
    double lon_diff = fabs(node_lon - last_lon);
    
    // Check if this is the first valid GPS fix
    bool is_first_gps_fix = !first_gps_fix_received && (node_lat != 0.0 || node_lon != 0.0);
    if (is_first_gps_fix) {
      first_gps_fix_received = true;
      MESH_DEBUG_PRINTLN("SensorManager: First GPS fix acquired - triggering immediate location share");
    }
    
    // Update if location changed by more than ~1 meter (approximately 0.00001 degrees)
    // or if this is the first valid GPS fix
    if ((lat_diff > 0.00001 || lon_diff > 0.00001) || 
        (last_lat == 0.0 && last_lon == 0.0)) {
      
      MESH_DEBUG_PRINTLN("SensorManager: GPS location changed significantly, updating location marker");
      MESH_DEBUG_PRINTLN("  Previous: lat=%.6f, lng=%.6f", last_lat, last_lon);
      MESH_DEBUG_PRINTLN("  Current:  lat=%.6f, lng=%.6f", node_lat, node_lon);
      MESH_DEBUG_PRINTLN("  Change:   lat=%.6f, lng=%.6f", lat_diff, lon_diff);
      
      // Update mesh location for sharing
      if (_mesh) {
        _mesh->setCurrentLocation(node_lat, node_lon);
        
        // Trigger immediate location share on first GPS fix
        if (is_first_gps_fix) {
          _mesh->triggerImmediateLocationShare();
        }
      }
      
      // Create/update location marker and emit fake advertisement
      if (!_mesh->createLocationMarker("📍", node_lat, node_lon)) {
        MESH_DEBUG_PRINTLN("Failed to create/update location marker");
      }
      
      // Update stored location
      last_lat = node_lat;
      last_lon = node_lon;
    }
  }
}

void SensorManager::onGPSUpdate() {
  if (!_mesh) {
    return; // No mesh instance available
  }
  
  // Check if this is the first valid GPS fix
  bool is_first_gps_fix = !first_gps_fix_received && (node_lat != 0.0 || node_lon != 0.0);
  if (is_first_gps_fix) {
    first_gps_fix_received = true;
    MESH_DEBUG_PRINTLN("SensorManager: First GPS fix acquired in onGPSUpdate - triggering immediate location share");
  }
  
  // Update mesh location for sharing when coordinates change
  double lat_diff = fabs(node_lat - last_lat);
  double lon_diff = fabs(node_lon - last_lon);
  
  if ((lat_diff > 0.00001 || lon_diff > 0.00001) || 
      (last_lat == 0.0 && last_lon == 0.0)) {
    
    MESH_DEBUG_PRINTLN("SensorManager: onGPSUpdate - location changed significantly");
    
    // Update mesh location for sharing
    _mesh->setCurrentLocation(node_lat, node_lon);
    
    // Trigger immediate location share on first GPS fix
    if (is_first_gps_fix) {
      _mesh->triggerImmediateLocationShare();
    }
    
    // Update stored location for comparison
    last_lat = node_lat;
    last_lon = node_lon;
  }
  
  // Always update location marker when onGPSUpdate is called
  updateLocationMarker();
}
