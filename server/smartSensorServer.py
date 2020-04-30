import busio
import time
from struct import *
from board import *
from adafruit_bus_device.i2c_device import I2CDevice
from gpiozero import LED
from Adafruit_IO import Client

TIME_BETWEEN_READINGS = 10

sensorsConfig = [
    { "name" : "Box 1", "id" : 1 },
    { "name" : "Box 2", "id" : 2 }
]

sensorsData = {
    "id1" : { "temp" : 0, "humidity" : 0 },
    "id2" : { "temp" : 0, "humidity" : 0 }    
}

led = LED(21)

def convert(byteData):
    stuff = unpack("ii", byteData)
    sensorData = {
        "temp" : stuff[0]/10,
        "humidity" : stuff[1]/10    
    }
    return sensorData

def sendData(id, sensorData):
    temp = sensorData["temp"]
    humidity = sensorData["humidity"]
    if (temp < -40 or temp > 100 or humidity < 0 or humidity > 100):
        print("oops, data rejected, not sending")
    else:
        try:
            aio.send("3d-temperature.temp-"+str(id), temp)
            aio.send("3d-temperature.humidity-"+str(id), humidity)
            print("Data sent to Adafruit IO")
        except RequestError:
            print("oops, can't send data to Adafruit IO")
        except ThrottlingError: 
            print("oops, reached max data for this minute")

aio = Client('xxxx', 'xxxx')

while True:
    for sensor in sensorsConfig:
        print("\nReading " + sensor["name"] + "...")
        sensorId = sensor["id"]
        with busio.I2C(SCL, SDA) as i2c:
            try:
                device = I2CDevice(i2c, sensorId)
                bytes_read = bytearray(8)
                with device:
                    device.readinto(bytes_read)
                    sensorData = convert(bytes_read)
                    temperature = sensorData["temp"]
                    humidity = sensorData["humidity"]
                    if (temperature < -50 or temperature > 50 or humidity < 0 or humidity > 100):
                        print("oops. bad data from sensor id %d" % (sensorId))
                    else:
                        sendData(sensorId, sensorData)
                    led.on()
                    print("temp: %4.1f, humidity: %4.1f" % (temperature, humidity))
            except ValueError:
                print("oops. can't read sensor id %d" % (sensorId))
                led.off()
            except IOError:
                print("oops. remote I/O error %d" % (sensorId))
                led.off()
            except RequestError:
                print("oops. request I/O error %d" % (sensor["id"]))
                led.off()
        time.sleep(TIME_BETWEEN_READINGS)
    
