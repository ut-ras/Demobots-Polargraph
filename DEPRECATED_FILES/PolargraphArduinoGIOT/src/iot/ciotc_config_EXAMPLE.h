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
// This file contains your configuration used to connect to Cloud IoT Core


// Originally from Esp8266-http example in https://github.com/GoogleCloudPlatform/google-cloud-iot-arduino
// private_key_str and fingerprint are from the example so replace with your own

// WIFI
const char* ssid = "";
const char* password = "";

// Cloud iot details.
const char* project_id = "";
const char* location = "us-central1";
const char* registry_id = "";
const char* device_id = "";


// To get the private key run (where private-key.pem is the ec private key
// used to create the certificate uploaded to google cloud iot):
// openssl ec -in ec_private.pem -noout -text
// and copy priv: part.
// The key length should be exactly the same as the key length bellow (32 pairs
// of hex digits). If it's bigger and it starts with "00:" delete the "00:". If
// it's smaller add "00:" to the start. If it's too big or too small something
// is probably wrong with your key.
const char* private_key_str =
    "5a:2e:06:b5:c1:f2:9c:b3:77:b2:89:f5:29:29:93:"
    "07:fd:ed:22:0d:03:2b:a6:b1:b6:04:0b:d5:9b:49:"
    "7d:ca";

// Use root certificate to verify tls connection rather than using a
// fingerprint.
// To get the fingerprint run
// openssl s_client -connect cloudiotdevice.googleapis.com:443 -cipher <cipher>
// Copy the certificate (all lines between and including ---BEGIN CERTIFICATE---
// and --END CERTIFICATE--) to a.cert. Then to get the fingerprint run
// openssl x509 -noout -fingerprint -sha1 -inform pem -in a.cert
// <cipher> is probably ECDHE-RSA-AES128-GCM-SHA256, but if that doesn't work
// try it with other ciphers obtained by sslscan cloudiotdevice.googleapis.com.
const char* fingerprint =
    "7C:D4:99:11:FE:FC:0D:78:C0:A9:C1:18:52:1D:3F:0B:8B:38:C9:90";
