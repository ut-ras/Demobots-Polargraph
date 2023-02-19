# Pathify SVG Files
# returns strings in format "(0,0)(0,0)"

import sys
from xml.dom import minidom
from svg.path import parse_path
from math import sqrt

"""
USER SETTINGS
	The output is scaled preserving aspect-ratio to [MAX_WIDTH,MAX_HEIGHT].
	MAX_WIDTH  : maximum width of output
	MAX_HEIGHT : maximum height of output
	max_line_len : maximum distance between points
	ROUND_TO_INTEGER : whether to round output values to the nearest integer
"""

MAX_WIDTH = 560
MAX_HEIGHT = 330
ROUND_TO_INTEGER = True

# utility function
def mround(num):
	if ROUND_TO_INTEGER:
		return round(num)
	return(num)

# string from SVG file (or seperate x and y strings, we can convert both to drawings)
def pathify_svg(svgfile, max_line_len, scale):
    svgfile_name = svgfile.filename
    svgfile_data = svgfile.read()
    print("Converting SVG from " + svgfile_name + " max_line_len=" + str(max_line_len))
    print("SVG Data: " + str(svgfile_data))
    pathstr = ""

    svg_dom = minidom.parseString(svgfile_data)
    path_strings = [path.getAttribute('d') for path in svg_dom.getElementsByTagName('path')]

    for path_string in path_strings:
        path_data = parse_path(path_string)

        x = []
        y = []

        for line in path_data:
            start = ((line.start.real), (line.start.imag))
            end = ((line.end.real), (line.end.imag))
            x.append(start[0])
            x.append(end[0])
            y.append(start[1])
            y.append(end[1])

        xmax = max(x)
        xmin = min(x)
        ymax = max(y)
        ymin = min(y)
        shift_x = -(xmax+xmin)/2
        shift_y = -(ymax+ymin)/2
        scale_x = xmax-xmin
        scale_y = ymax-ymin
        if scale_x/MAX_WIDTH > scale_y/MAX_HEIGHT:
            scale_x = MAX_WIDTH/scale_x
            scale_y = scale_x
        else:
            scale_y = MAX_HEIGHT/scale_y
            scale_x = scale_y

        for index, item in enumerate(x):
            x[index] = mround((x[index]+shift_x)*scale_x*scale)
            y[index] = mround((y[index]+shift_y)*-scale_y*scale)

        # remove duplicates
        i = 0
        while i < len(x)-1:
            if x[i] == x[i+1] and y[i] == y[i+1]:
                del x[i+1]
                del y[i+1]
            else:
                i += 1

        for index, item in enumerate(x):
            code = '({},{}),'.format(x[index], y[index])
            pathstr += code

    print("SVG Conversion Complete")
    return pathstr


# TODO cleanup
def pathify_svg_curve_extraction(svgfile, max_line_len, scale):
    svgfile_name = svgfile.filename
    svgfile_data = svgfile.read()
    print("Converting SVG from " + svgfile_name + " max_line_len=" + str(max_line_len) + " scale=" + str(scale))
    #print("SVG Data: " + str(svgfile_data))
    pathstr = ""

    svg_dom = minidom.parseString(svgfile_data)
    path_strings = [path.getAttribute('d') for path in svg_dom.getElementsByTagName('path')]

    for path_string in path_strings:
        path_data = parse_path(path_string)

        # find scale
        x = []
        y = []
        for line in path_data:
            start = (line.start.real, line.start.imag)
            end = (line.end.real, line.end.imag)
            x.append(line.start.real)
            x.append(line.end.real)
            y.append(line.start.imag)
            y.append(line.end.imag)

        # scale points
        xmin = min(x)
        xmax = max(x)
        ymin = min(y)
        ymax = max(y)
        shift_x = -(xmax+xmin)/2 # will center x by shifting by this amount
        shift_y = -(ymax+ymin)/2 # will center y by shifting by this amount
        scale_x = xmax-xmin
        scale_y = ymax-ymin
        if scale_x/MAX_WIDTH > scale_y/MAX_HEIGHT:
            scale_x = MAX_WIDTH/scale_x
            scale_y = scale_x
        else:
            scale_y = MAX_HEIGHT/scale_y
            scale_x = scale_y

        #curve extraction
        x = []
        y = []
        for line in path_data:
            # first, find
            num_segments = 1
            error = max_line_len * 2
            start = (line.start.real*scale_x*scale, line.start.imag*-scale_y*scale)
            #while error > max_line_len and num_segments < 1000:
            #    num_segments += 1
            #    second = (line.point(1/num_segments).real, line.point(1/num_segments).imag)
            #    error = sqrt((start[0]-second[0])**2 + (start[1]-second[1])**2)
            #    start = second
            end = (line.end.real*scale_x*scale, line.end.imag*-scale_y*scale)
            dist = sqrt((start[0]-end[0])**2 + (start[1]-end[1])**2)

            if dist <= max_line_len:
                num_segments = 1
            else:
                num_segments = int(dist / max_line_len)

            for i in range(num_segments+1):
                point = line.point(i/num_segments)
                x.append(((point.real+shift_x)*scale_x*scale))
                y.append(((point.imag+shift_y)*scale_y*scale))

        # scale again to ensure perfect fit; will only scale down
        xmin = min(x)
        xmax = max(x)
        ymin = min(y)
        ymax = max(y)
        shift_x = -(xmax+xmin)/2 # will center x by shifting by this amount
        shift_y = -(ymax+ymin)/2 # will center y by shifting by this amount
        scale_x = xmax-xmin
        scale_y = ymax-ymin
        if scale_x/MAX_WIDTH > scale_y/MAX_HEIGHT:
            scale_x = MAX_WIDTH/scale_x
            scale_y = scale_x
        else:
            scale_y = MAX_HEIGHT/scale_y
            scale_x = scale_y

        for index, item in enumerate(x):
            x[index] = mround((x[index]+shift_x)*scale_x*scale)
            y[index] = mround((y[index]+shift_y)*-scale_y*scale) # flip y axis

        # remove duplicates
        i = 0
        while i < len(x)-1:
            if x[i] == x[i+1] and y[i] == y[i+1]:
                del x[i+1]
                del y[i+1]
            else:
                i += 1

        for index, item in enumerate(x):
            code = '({},{}),'.format(x[index], y[index])
            pathstr += code

    print("SVG Conversion Complete")
    return pathstr
