/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
// This file throttles with a backoff mechanism


// Originally from Esp8266-http example in https://github.com/GoogleCloudPlatform/google-cloud-iot-arduino
/* Modifications from Original:
 * Changed the min and max
 * Lowered the delay()
 */

#ifndef __BACKOFF_H__
#define __BACKOFF_H__

int backOffCount = 0;
long minBackoff = 1000; // 1000 if you don't mind sending lots of data
long maxBackoff = 20000;
long minJitter = 50;
long maxJitter = 1000;

long currDelay = minBackoff;
long lastRequestTime = millis();

void resetBackoff() {
  backOffCount = 0;
}

// Returns true if the backoff duration has passed
bool backoff() {
  if ((millis() - lastRequestTime) > currDelay) {
    backOffCount++;
    currDelay = (backOffCount * backOffCount * minBackoff) + random(minJitter,maxJitter);
    if (currDelay > maxBackoff) {
      currDelay = maxBackoff;
    }
    Serial.printf("Waiting: %ld\n", currDelay);
    delay(100); // FIXME remove
    lastRequestTime = millis();
    return true;
  }
  return false;
}

#endif
