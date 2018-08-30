from polargraph_app import oauth2
from flask import Blueprint, current_app, redirect, render_template, request, session, url_for
from . import crud_cloudsql
from . import iot_manager
import io
import datetime
import time
import base64
import ast

control_blueprint = Blueprint('control_blueprint', __name__)


@control_blueprint.route("/", methods=['GET', 'POST'])
#@oauth2.required
def home():
    #if not crud_cloudsql.is_user_admin():
    #    return render_template("not_admin.html")

    state = iot_manager.get_state(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID'])

    state['date'] = state['updateTime'][0:10]
    state['time'] = utc_to_local_time(state['updateTime'][11:19])

    state['connected'] = ( (state['date'] == str(datetime.datetime.utcnow().date())) and (int(state['updateTime'][11:13]) == int(datetime.datetime.utcnow().hour)) and (int(state['updateTime'][14:16]) >= int(datetime.datetime.utcnow().minute) - 1) )
    conf = iot_manager.get_config(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID'])
    coordinates = "(0,0)(0,0)"

    profile = "Not Logged In"
    if 'profile' in session:
        profile = session['profile']

    if request.method == 'POST':
        data = request.form.to_dict(flat=True)

        if 'conf' in data:
            print("update config: " + data['conf'])
            iot_manager.set_config(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID'], 0, data['conf'])
            conf['data'] = iot_manager.dict_from_str(data['conf'])
            return render_template("control.html", conf=conf, state=state, coordinates=coordinates)

        if 'coordinates' in data:
            coordinates = data['coordinates']
            conf_str = config_from_coords(coordinates)
            print("add coordinates: " + conf_str)
            iot_manager.set_config(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID'], 0, conf_str)
            conf['data'] = iot_manager.dict_from_str(conf_str)
            return render_template("control.html", coordinates=coordinates, conf=conf, state=state)

        if 'clearBuffer' in data:
            conf_str = "{'t':'" + str(datetime.datetime.now()) + "','d':'false','cb':'true',}"
            print("clearBuffer: " + conf_str)
            iot_manager.set_config(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID'], 0, conf_str)
            conf['data'] = iot_manager.dict_from_str(conf_str)
            return render_template("control.html", conf=conf, state=state, coordinates=coordinates)

        if 'startDrawing' in data:
            conf_str = "{'t':'" + str(datetime.datetime.now()) + "','d':'true','cb':'false',}"
            print("startDrawing: " + conf_str)
            iot_manager.set_config(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID'], 0, conf_str)
            conf['data'] = iot_manager.dict_from_str(conf_str)
            return render_template("control.html", conf=conf, state=state, coordinates=coordinates)

        if 'stopDrawing' in data:
            conf_str = "{'t':'" + str(datetime.datetime.now()) + "','d':'false','cb':'false',}"
            print("stopDrawing: " + conf_str)
            iot_manager.set_config(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID'], 0, conf_str)
            conf['data'] = iot_manager.dict_from_str(conf_str)
            return render_template("control.html", conf=conf, state=state, coordinates=coordinates)

    return render_template("control.html", conf=conf, state=state, coordinates=coordinates)


# local_time_from_utc_timestamp
# time format "HH:MM:SS"
def utc_to_local_time(utc_time):
    offset = time.timezone if (time.localtime().tm_isdst == 0) else time.altzone
    offset = int(offset / 60 / 60 * -1)
    hour = int(utc_time[0:2]) + offset
    if hour > 24:
        hour -= 24
    elif hour < 1:
        hour += 24
    local_time = str(hour) + utc_time[2:8]
    return local_time


def config_from_coords(coordinates):
    conf = "{'t':'" + str(datetime.datetime.now()) + "',"
    x_str = "'bx':'{"
    y_str = "'by':'{"

    start_parenthesis_index = 0
    while start_parenthesis_index != -1:
        comma_index = coordinates.find(",", start_parenthesis_index)
        end_parenthesis_index = coordinates.find(")", comma_index)
        x_str += coordinates[(start_parenthesis_index + 1):comma_index] + ","
        y_str += coordinates[(comma_index + 1):end_parenthesis_index] + ","
        start_parenthesis_index = coordinates.find("(", end_parenthesis_index)

    y_str += "}',"
    x_str += "}',"
    conf += x_str + y_str + "'cb':'false',}"
    return conf
