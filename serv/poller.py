from datetime import datetime
import time
import mushroom_controller


def run(conn, filename):
    with open(filename, "w") as file:
        print("erasing file")
        file.write("date, time, expected temp, actual temp, expected hygro, actual hygro\n")
    while True:
        data = []
        exp_tmp = str(mushroom_controller.get_expected_tmp(conn))
        exp_hygro = str(mushroom_controller.get_expected_hygro(conn))
        actual_tmp = str(mushroom_controller.get_actual_tmp(conn))
        actual_hygro = str(mushroom_controller.get_actual_hygro(conn))
        stamp = datetime.now().strftime("%d/%m/%Y, %H:%M:%S")
        data = ", ".join((stamp, exp_tmp, actual_tmp, exp_hygro, actual_hygro))
        with open(filename, "a") as file:
            file.write(data + "\n")
        print("writing", data)
        time.sleep(1)
