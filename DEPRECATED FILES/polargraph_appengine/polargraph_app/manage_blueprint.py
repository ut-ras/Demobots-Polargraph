from polargraph_app import oauth2
from flask import Blueprint, current_app, redirect, render_template, request, session, url_for
import datetime
import time
import base64
import ast
from threading import Thread

from . import crud_cloudsql
from . import iot_manager
from . import pathify_svg



manage_blueprint = Blueprint('manage_blueprint', __name__)

@manage_blueprint.route("/")
#@oauth2.required
def home():
    #if not crud_cloudsql.is_user_admin():
    #    return render_template("not_admin.html")
    token = request.args.get('page_token', None)
    if token:
        token = token.encode('utf-8')
    drawings, next_page_token = crud_cloudsql.list_drawings(cursor=token)
    return render_template("manage.html", drawings=drawings, next_page_token=next_page_token)


@manage_blueprint.route('/add', methods=['GET', 'POST'])
@oauth2.required
def add():
    if not crud_cloudsql.is_user_admin():
        return render_template("not_admin.html")
    if request.method == 'POST':
        data = request.form.to_dict(flat=True)
        if data['coord_input'] == 'xandy':
            data = drawing_from_input_xandy(data)
        elif data['coord_input'] == 'coords':
            data = drawing_from_input_coords(data)
        elif data['coord_input'] == 'svg':
            data = drawing_from_input_svg(data, request.files.get('svgfile'))
        drawing = crud_cloudsql.create_drawing(data)
        return redirect(url_for('.home'))
    return render_template("drawing.html", action="Add", drawing={})


@manage_blueprint.route('/<id>/edit', methods=['GET', 'POST'])
@oauth2.required
def edit(id):
    if not crud_cloudsql.is_user_admin():
        return render_template("not_admin.html")
    if request.method == 'POST':
        data = request.form.to_dict(flat=True)
        if data['coord_input'] == 'xandy':
            data = drawing_from_input_xandy(data)
        elif data['coord_input'] == 'coords':
            data = drawing_from_input_coords(data)
        elif data['coord_input'] == 'svg':
            data = drawing_from_input_svg(data, request.files.get('svgfile'))
        drawing = crud_cloudsql.update_drawing(data, id)
        return redirect(url_for('.home'))
    drawing = crud_cloudsql.read_drawing(id)
    return render_template("drawing.html", action="Edit", drawing=drawing)


@manage_blueprint.route('/<id>/delete')
@oauth2.required
def delete(id):
    if not crud_cloudsql.is_user_admin():
        return render_template("not_admin.html")
    crud_cloudsql.delete_drawing(id)
    return redirect(url_for('.home'))

@manage_blueprint.route('/<id>/draw')
@oauth2.required
def draw(id):
    if not crud_cloudsql.is_user_admin():
        return render_template("not_admin.html")
    config_loop_drawing(crud_cloudsql.read_drawing(id), (60 * 3), 150, 4)
    return redirect(url_for('control_blueprint.home'))




# config_loop_drawing: sends a drawing as a config
# takes a max # of coordinates for one transmission and a timeout
# this loop is meant to handle the case when there are more coordinates in the drawing than the Arduino can handle in one config
# separates the coordinates into different configs, sends one config and waits for response from Arduino before sending the next
# TODO right now this will take a really long time for the back-forth communication when greater than max coords,
#           might need to start a thread or task queue so the page doesnt hang
def config_loop_drawing(drawing, timeout, max_coords, version_check_sleep):
    end_t = time.time() + timeout

    temp_d = drawing
    coordinatesX = drawing["coordinatesX"]
    coordinatesY = drawing["coordinatesY"]
    if coordinatesX[len(coordinatesX) - 1] != ",":
        coordinatesX += ","
    if coordinatesY[len(coordinatesY) - 1] != ",":
        coordinatesY += ","

    comma_indexes_x = [i for i, n in enumerate(coordinatesX) if n == ',']
    comma_indexes_y = [i for i, n in enumerate(coordinatesY) if n == ',']
    comma_indexes_x.insert(0, -1)
    comma_indexes_y.insert(0, -1)
    num_coords = len(comma_indexes_x)
    comma_index = 0
    new_version = -1

    # send coords in increments of max_coords. for each set of coords, update config, figure out new config number, and wait until device state reports new config
    while comma_index < num_coords - 1:
        next_comma_index = comma_index + max_coords
        if next_comma_index >= num_coords:
            next_comma_index = num_coords - 1

        # temp_d is a subset of drawing
        temp_d["coordinatesX"] = coordinatesX[comma_indexes_x[comma_index] + 1:comma_indexes_x[next_comma_index] + 1]
        temp_d["coordinatesY"] = coordinatesY[comma_indexes_y[comma_index] + 1:comma_indexes_y[next_comma_index] + 1]

        print("--------------- Sending coords " + str(comma_index) + "-" + str(next_comma_index - 2) + "/" + str(num_coords - 1) + " in a config")
        print("--------------- coordinatesX: " + temp_d["coordinatesX"])
        print("--------------- coordinatesY: " + temp_d["coordinatesY"])

        comma_index = next_comma_index
        startDraw = str(comma_index == num_coords - 1)
        conf = config_from_drawing(temp_d, startDraw)
        iot_manager.set_config(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID'], 0, conf)
        time.sleep(0.5)

        if comma_index < num_coords - 1:
            if new_version == -1:
                new_version = int(iot_manager.get_config(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID']).get('version'))
            else:
                new_version = new_version + 1

            # waiting for device state to reflect new config
            device_version = '0';
            while int(new_version) != int(device_version):
                state = iot_manager.get_state(current_app.config['SERVICE_ACCOUNT_JSON'], current_app.config['PROJECT_ID'], current_app.config['CLOUD_REGION'], current_app.config['REGISTRY_ID'], current_app.config['DEVICE_ID'])
                device_version = state['data']['last_conf']
                print("--------------- Waiting for update... new_version:" + str(new_version) + " device_version:" + str(device_version))
                if time.time() >= end_t:
                    print("--------------- Drawing loop timed out after " + str(comma_index) + " coords")
                    return
                time.sleep(version_check_sleep)

    print("--------------- Finished sending coords")


# converts a drawing dict into a config string
def config_from_drawing(drawing, startDraw):
    conf = "{'t':'" + str(datetime.datetime.now()) + "',"

    x_str = "'bx':'{" + drawing["coordinatesX"]
    y_str = "'by':'{" + drawing["coordinatesY"]

    if x_str[len(x_str) - 1] != ",":
        x_str += ","

    if y_str[len(y_str) - 1] != ",":
        y_str += ","

    y_str += "}',"
    x_str += "}',"

    conf += x_str + y_str + "'d':'" + startDraw + "',}"
    return conf



# new drawing inputs

def drawing_from_input_xandy(data):
    drawing = {}
    drawing["coordinatesX"] = data["coordinatesX"]
    drawing["coordinatesY"] = data["coordinatesY"]
    drawing["title"] = data['title']
    drawing["numPoints"] = data["numPoints"]
    return drawing

def drawing_from_input_coords(data):
    coordinates = data['coordinates']
    coordinatesX = ""
    coordinatesY = ""
    num_coordinates = 0

    start_parenthesis_index = 0
    while start_parenthesis_index != -1:
        comma_index = coordinates.find(",", start_parenthesis_index)
        end_parenthesis_index = coordinates.find(")", comma_index)
        coordinatesX += coordinates[(start_parenthesis_index + 1):comma_index] + ","
        coordinatesY += coordinates[(comma_index + 1):end_parenthesis_index] + ","
        start_parenthesis_index = coordinates.find("(", end_parenthesis_index)
        num_coordinates = num_coordinates + 1

    drawing = {}
    drawing["coordinatesX"] = coordinatesX
    drawing["coordinatesY"] = coordinatesY
    drawing["title"] = data['title']
    drawing["numPoints"] = num_coordinates
    return drawing

def drawing_from_input_svg(data, svgfile):
    print(data)
    max_line_len = 5
    scale = 1.0
    if 'max_line_len' in data:
        max_line_len = int(data['max_line_len'])

    if 'scale' in data:
        scale = float(data['scale'])

    if 'ce_check' in data:
        data['coordinates'] = pathify_svg.pathify_svg_curve_extraction(svgfile, max_line_len, scale)
    else:
        data['coordinates'] = pathify_svg.pathify_svg(svgfile, max_line_len, scale)
    return drawing_from_input_coords(data)
