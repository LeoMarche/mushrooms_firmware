from datetime import datetime
import time
import json
import mushroom_controller


def run(conn, filename, lock):
    with open(filename, "w") as file:
        print("erasing file")
        file.write("date time, Temp. attendue, Temp. effective, Humidité attendue, Humidité effective\n")
    while True:
        data = []
        with lock:
            exp_tmp = str(mushroom_controller.get_expected_tmp(conn))
            exp_hygro = str(mushroom_controller.get_expected_hygro(conn))
            actual_tmp = str(mushroom_controller.get_actual_tmp(conn))
            actual_hygro = str(mushroom_controller.get_actual_hygro(conn))
        stamp = datetime.now().strftime("%Y-%m-%dT%H:%M:%S")
        data = ", ".join((stamp, exp_tmp, actual_tmp, exp_hygro, actual_hygro))
        with open(filename, "a") as file:
            file.write(data + "\n")

        try:
            with open("prog.json", "r") as file:
                jsondata = json.load(file)
        except Exception as e:
            pass
        else:
            to_do = None
            while len(jsondata) > 0 and jsondata[-1][0] < stamp:  # if should be applied
                to_do = jsondata.pop()
            with open("prog.json", "w") as file:
                json.dump(jsondata, file)
            if to_do is not None:
                with lock:
                    mushroom_controller.set_hygro(conn, to_do[1]["hygro"])
                    mushroom_controller.set_temperature(conn, to_do[1]["temp"])

        time.sleep(1)
