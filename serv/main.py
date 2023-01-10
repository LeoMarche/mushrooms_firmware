import os
import csv
import poller
import json
import mushroom_controller
from datetime import datetime
from flask import Flask, render_template, request


from threading import Thread
from threading import Lock


# create a shared lock
lock = Lock()
conn = mushroom_controller.get_connection()
print(">>>", conn is None)
filename = "file.csv"

Thread(target=poller.run, args=(conn, filename, lock)).start()
# if os.fork() == 0:
#     poller.run(conn, filename)
#     os._exit(1)  # unreachable


app = Flask(__name__)


@app.route("/")
def homepage():
    return render_template('index.html')


@app.route("/data")
def get_data():
    with open(filename, "r") as file:
        raw = [v for v in csv.reader(file)]
        data = [
            [raw[i][j] for i in range(len(raw))]
            for j in range(len(raw[0]))]
    return {
        "data": [
            {
                "name": line[0],
                "x": data[0][1:],
                "y": line[1:],
            }
            for line in data[1:]],
        "time": datetime.now().strftime("%Y-%m-%dT%H:%M:%S")
    }


@app.route("/set_settings", methods=['POST'])
def send_settings():
    data = request.get_json()
    with lock:
        mushroom_controller.set_hygro(conn, data["hygro"])
        print("set hydro")
        mushroom_controller.set_temperature(conn, data["temp"])
        print("set temp")
    return "Ok"


@app.route("/program", methods=['POST'])
def program():
    data = request.get_json()
    try:
        with open("prog.json", "r") as file:
            jsondata = json.load(file)
    except Exception:
        jsondata = []
    for val in data:
        try:
            date = datetime.strptime(val["date"], '%Y-%m-%dT%H:%M:%S')
        except Exception:
            return "Bad program data", 400
        if type(val["hygro"]) != float or type(val["temp"]) != float:
            return "Bad program data", 400
        jsondata.append([
            date.strftime("%Y-%m-%dT%H:%M:%S"),
            {"hygro": val["hygro"], "temp": val["temp"]}])
    jsondata.sort(reverse=True)
    with open("prog.json", "w+") as file:
        json.dump(jsondata, file)
    return "Ok"


@app.route("/program", methods=['DELETE'])
def clear_program():
    with open("prog.json", "w+") as file:
        file.write("[]")
    return "Ok"
