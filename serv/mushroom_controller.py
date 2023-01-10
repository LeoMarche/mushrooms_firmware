import serial
import serial.tools.list_ports
import time

PORT = "/dev/ttyUSB0"


def get_connection():
    global PORT
    try:
        ser = serial.Serial(PORT, baudrate=9600, timeout=10)
        time.sleep(5)
        if ser is None:
            raise Exception("Could not connect to hardware")
        return ser
    except Exception as e:
        print(e)
        raise Exception("Could not connect to hardware")


def set_temperature(conn, temp):
    to_send = '1 '+str(temp)+"\r\n"
    conn.write(to_send.encode())
    c = conn.readline().rstrip()
    try:
        return float(c)
    except Exception:
        return None


def set_hygro(conn, hygro):
    to_send = '2 '+str(hygro)+"\r\n"
    conn.write(to_send.encode())
    c = conn.readline().rstrip()
    try:
        return float(c)
    except Exception:
        return None


def get_expected_tmp(conn):
    conn.write(b"3\n")
    c = conn.readline().rstrip()
    try:
        return float(c)
    except Exception:
        return None


def get_expected_hygro(conn):
    conn.write(b"4\n")
    c = conn.readline().rstrip()
    try:
        return float(c)
    except Exception:
        return None


def get_actual_tmp(conn):
    conn.write(b"5\n")
    c = conn.readline().rstrip()
    try:
        return float(c)
    except Exception:
        return None


def get_actual_hygro(conn):
    conn.write(b"6\n")
    c = conn.readline().rstrip()
    try:
        return float(c)
    except Exception:
        return None


# Testing connection
if __name__ == "__main__":
    conn = get_connection()
    print("===DEFAULT COMMAND===")
    print("Temperature command before : ", get_expected_tmp(conn))
    print("Hygro command before : ", get_expected_hygro(conn))
    print("===UPDATING COMMANDS===")
    print("Setting temperature command to : ", set_temperature(conn, 25.0))
    print("Setting hygro command to : ", set_hygro(conn, 60.0))
    print("===COMMANDS AFTER UPDATING===")
    print("Temperature command after : ", get_expected_tmp(conn))
    print("Hygro command after : ", get_expected_hygro(conn))
    print("===CURRENT VALUES===")
    print("Current temperature : ", get_actual_tmp(conn))
    print("Current Hygro : ", get_actual_hygro(conn))
