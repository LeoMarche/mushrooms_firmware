import os
import csv
import poller
import mushroom_controller
from flask import Flask, render_template

conn = mushroom_controller.get_connection()
filename = "file.csv"

if os.fork() == 0:
    poller.run(conn, filename)
    os._exit(1)  # unreachable


app = Flask(__name__)


@app.route("/")
def hello_world():
    with open(filename, "r") as file:
        data = [x for i, x in enumerate(csv.reader(file)) if i > 0]
    return "<br/>".join([str(x) for x in data])
