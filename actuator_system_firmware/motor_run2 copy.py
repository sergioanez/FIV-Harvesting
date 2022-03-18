from math import sin, radians
from time import sleep

a= 1000
w = 4*2*3.14
def wave():
	for i in range(-180*20, 180*20):
            angle = radians(i*w/20)
            s = round( float( "{:.02f}".format( a*sin( angle ) * 100 ) ) ) // 2
            print(f"{angle} degrees:"+ f"{s}", end="\n")
            sleep(0.01)

while True:
    wave()
  
