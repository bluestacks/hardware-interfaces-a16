/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MockLocation.h"
#include <sys/system_properties.h>
#include <cstdio>
#include <cmath>

namespace android::hardware::gnss::common {

// BS-A16: default mock location (Googleplex). Overridden at runtime when
// bst.config.sysLoc ("lat,lng") is set by BstCommandProcessor / FakeGps.
float gMockLatitudeDegrees{37.4219999};
float gMockLongitudeDegrees{-122.0840575};
float gMockAltitudeMeters{1.60062531};
float gMockBearingDegrees{0.0001};  // a real location rarely has exactly zero bearing
float gMockSpeedMetersPerSec{0.0001};

}  // namespace android::hardware::gnss::common

namespace android::hardware::gnss::common::bst {

// Returns true if bst.config.sysLoc is set to a valid non-(0,0) location.
// When true, outLat/outLon contain the override coordinates.
bool getSysLocOverride(double* outLat, double* outLon) {
    char prop[92] = {0};
    int len = __system_property_get("bst.config.sysLoc", prop);
    if (len <= 0 || prop[0] == '\0') return false;
    double lat = 0.0, lon = 0.0;
    if (sscanf(prop, "%lf,%lf", &lat, &lon) != 2) return false;
    if (lat == 0.0 && lon == 0.0) return false;
    if (std::isnan(lat) || std::isnan(lon)) return false;
    *outLat = lat;
    *outLon = lon;
    return true;
}

}  // namespace android::hardware::gnss::common::bst
