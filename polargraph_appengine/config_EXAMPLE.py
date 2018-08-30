import io
import os

# Make sure to enbale all APIs
# Google+ API, Cloud IoT API, Cloud SQL Admin API

# The secret key is used by Flask to encrypt session cookies.
SECRET_KEY = ''     #generate from os.random(24)

# Google Cloud Project ID. This can be found on the 'Overview' page at
# https://console.developers.google.com
PROJECT_ID = ''
CLOUD_REGION = 'us-central1'

# Cloud IOT
DEVICE_ID = ''
REGISTRY_ID = ''
SERVICE_ACCOUNT_JSON = 'keys/jsoncert.json'


# CloudSQL & SQLAlchemy configuration
# To connect to database from Google Cloud Console:
# $ gcloud sql connect cloudsql-instance --user=root
# Examplke SQL: initializing admins table
# $ USE dbase;
# $ CREATE TABLE admins (email VARCHAR(255), PRIMARY KEY(email));
# Example SQL: adding email to admins
# $ USE dbase;
# $ INSERT INTO admins (email) values ("email@gmail.com");
# $ SELECT * FROM admins;
# Replace the following values the respective values of your Cloud SQL instance.
CLOUDSQL_USER = 'root'
CLOUDSQL_PASSWORD = ''
CLOUDSQL_DATABASE = ''
# Set this value to the Cloud SQL connection name, e.g.
#   "project:region:cloudsql-instance".
# You must also update the value in app.yaml.
CLOUDSQL_CONNECTION_NAME = 'project:region:cloudsql-instance'

# The CloudSQL proxy is used locally to connect to the cloudsql instance.
# To start the proxy, use:
#   ./cloud_sql_proxy -instances=project:region:cloudsql-instance=tcp:3306
# Port 3306 is the standard MySQL port. If you need to use a different port,
# change the 3306 to a different port number.

# Alternatively, you could use a local MySQL instance for testing.
LOCAL_SQLALCHEMY_DATABASE_URI = (
    'mysql+pymysql://{user}:{password}@127.0.0.1:3306/{database}').format(
        user=CLOUDSQL_USER, password=CLOUDSQL_PASSWORD,
        database=CLOUDSQL_DATABASE)

# When running on App Engine a unix socket is used to connect to the cloudsql
# instance.
LIVE_SQLALCHEMY_DATABASE_URI = (
    'mysql+pymysql://{user}:{password}@localhost/{database}'
    '?unix_socket=/cloudsql/{connection_name}').format(
        user=CLOUDSQL_USER, password=CLOUDSQL_PASSWORD,
        database=CLOUDSQL_DATABASE, connection_name=CLOUDSQL_CONNECTION_NAME)

if os.environ.get('GAE_INSTANCE'):
    SQLALCHEMY_DATABASE_URI = LIVE_SQLALCHEMY_DATABASE_URI
else:
    SQLALCHEMY_DATABASE_URI = LOCAL_SQLALCHEMY_DATABASE_URI



# Enable Google Plus API for profile info
# OAuth2 configuration.
# This can be generated from the Google Developers Console at
# https://console.developers.google.com/project/_/apiui/credential.
# Note that you will need to add all URLs that your application uses as
# authorized redirect URIs. For example, typically you would add the following:
#  * http://localhost:8080/oauth2callback
#  * https://<your-app-id>.appspot.com/oauth2callback.
# If you receive a invalid redirect URI error review you settings to ensure
# that the current URI is allowed.
GOOGLE_OAUTH2_CLIENT_ID = ''
GOOGLE_OAUTH2_CLIENT_SECRET = ''
