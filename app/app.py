from flask import Flask, request, render_template, send_file, jsonify
from grapher import *
from utils import *
import tempfile
from io import BytesIO
import json
app = Flask(__name__)

dataloc = '/data/'
in_out_map = {'input': '1', 'output': '0'}

@app.route('/', methods=["GET", "POST"])
def gfg():
    return render_template("index.html")

@app.route('/getVariableNames/', methods=['POST'])
def getVarNames():
    body = request.get_json()
    path = body['ncpath']
    var_type = body['type']
    if var_type == 'input':
        return jsonify(getVariableNames(dataloc + '1/' + path + '.nc'))
    elif var_type == 'output':
        return jsonify(getVariableNames(dataloc + '0/' + path + '.nc'))
    else:
        return jsonify([])

@app.route('/render/', methods=["GET", "POST"])
def render():
    if request.method == 'POST': 
        body = request.get_json()

        var_type = in_out_map[body['type']]

        ncpath = body['ncpath']
        variable =  body['variable']
        date = body['date']
        sdate = str(date).split('-')
        date = sdate[1]
        date += "/"
        date += sdate[2]
        date += "/"
        date += sdate[0]

        hour = body['hour']
        fig = graph(dataloc + f'{var_type}/' + ncpath + ".nc", variable, date, hour, ncpath)

        image_data = BytesIO()
        fig.savefig(image_data, format='png')
        plt.close(fig)
        image_data.seek(0)
        return send_file(image_data, mimetype='image/png')

@app.route('/data/', methods=["POST"])
def get_data():
    body = request.get_json()

    var_type = in_out_map[body['type']]
    ncpath = body['ncpath']
    variable =  body['variable']
    date = body['date']
    hour = body['hour']
        
    numpy_arr = get_numpy(dataloc + f'{var_type}/' + ncpath + ".nc", variable, date, hour)

    return jsonify(numpy_arr.tolist())

if __name__ == "__main__":
    app.run(host='0.0.0.0')
