# Demobots Polargraph IOT
UT Austin's R.A.S. Demobots made an Arduino polargraph on an ESP8266, and this example adds a Google App Engine front end and communication through Cloud IoT Manager's HTTP bridge. </br>
</br>
Currently hosted on utras-demobots.appspot.com but you must be an admin to control anything.</br>
</br>
Cloud IOT provides a simple and secure REST API (cloudiotdevice) for the device to post state and get config to the device manager. The device manager stores some of the most recent states and configs, and another API (cloudiot) can access the device manager. </br>
</br>
Google AppEngine is a platform for building webapps, it will host your webapp and five you access to some diagnostic tools. </br>

## Google Cloud Tutorials
AppEngine Tutorial: Setting up gcloud development environment, python bookshelf app, Flask, Jinja, OAuth, CloudSQL</br>
https://cloud.google.com/python/getting-started/tutorial-app</br>
Cloud IoT How-To's: Config, state, HTTP bridge </br>
https://cloud.google.com/iot/docs/how-tos/getting-started</br>
Cloud IoT REST Reference: APIs for the HTTP device manager (cloudiot) HTTP bridge (cloudiotdevice)</br>
https://cloud.google.com/iot/docs/reference/rest/</br>
Cloud IoT Manager Example: Using python client library to connect to IoT device manager with cloudiot API</br>
https://github.com/GoogleCloudPlatform/python-docs-samples/tree/master/iot/api-client/manager</br>
Cloud IoT Core JWT Arduino library: Cloud IoT end device examples on Arduino, JWT, receiving config, updating state, sending telemetry to PubSub with cloudiotdevice API</br>
https://github.com/GoogleCloudPlatform/google-cloud-iot-arduino</br>

## Arduino Libraries
ESP8266WiFi</br>
Google Cloud IoT Core JWT</br>
AccelStepper</br>
rBase64</br>
QueueList</br>

## Google Cloud Services
Google App Engine (python with Flask, Jinja, SQLAlchemy)</br>
OAuth 2.0 </br>
Cloud SQL </br>
Cloud IoT Manager with HTTP bridge</br>

## Google Cloud Console Stuff to Set Up
Things to do when setting up this project. In general, follow the examples and tutorials.
* Google Cloud Project
* Cloud SQL Instance and drawings Database
* Cloud IOT Registry and Device with EC Public Private Key Pair and Service Account
* Enable all API's - Cloud IoT, AppEngine Flexible, Cloud SQL, CLoud SQL Admin, Google+, (might be more)
* Config files were not committed for security reasons, so make copies of everything with "\_EXAMPLE", remove the "\_EXAMPLE", and fill in your info, read comments for instructions

## Running AppEngine Locally (Linux)
First, make sure that project is set up like in Bookshelf tutorial, Google Cloud SDK is installed, and config.py/app.yaml are set up.</br>
virtualenv -p python3 env</br>
source env/bin/activate</br>
pip install -r requirements.txt</br>
python main.py</br>

## Running Cloud SQL Locally (Linux)
Install Proxy:</br>
wget https://dl.google.com/cloudsql/cloud_sql_proxy.linux.amd64 -O cloud_sql_proxy</br>
chmod +x cloud_sql_proxy</br>
Run Proxy:</br>
./cloud_sql_proxy -instances=project-name:us-central1:sql-name=tcp:3306</br>


## TODO
Come to Demobots meetings or message R.A.S. if you want to participate! Any ideas are welcome. We are going to set up a more official task system in the future, but for now here are some tasks.

#### Arduino:
* Move the update state/config HTTP requests onto a separate thread from the Steppers
* Serial Mode (takes config as string)
* Sometimes crashes with large configs, even when they are small enough for the device manager and the base64 conversion buffer. We are splitting drawings into multiple configs when necessary but we should investigate and try to increase max size to match the max for the IoT HTTP bridge (64kb) to minimize the number of transmissions. 

#### Cloud:
* Maybe using device metadata would be helpful for stuff like polargraph size, wifi connection settings, etc
* PubSub could be cool for drawings instead of using config
* Cloud IoT also has an MQTT bridge we can try

#### AppEngine:
* Editable canvas in Add Drawing
* SVG polargraph size text input
* Background thread or task queue when number of coordinates needs multiple configs

#### Hardware:
* New pen holder, possibly with a small servo to lift/place the pen
* Big reset / stop / motor power buttons (use interrupts for stop)
* Surface for drawing with dry erase markers
* Find an appropriate battery pack for outdoor demos
