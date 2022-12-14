import time
import math 
from grove.i2c import Bus

ADC_DEFAULT_IIC_ADDR = 0X04
ADC_CHAN_NUM = 8
REG_RAW_DATA_START = 0X10
REG_VOL_START = 0X20
REG_RTO_START = 0X30
REG_SET_ADDR = 0XC0
__all__ = ['Current','Bus']

class Current():

    #Define self bus address

    def __init__(self,bus_num=1,addr=ADC_DEFAULT_IIC_ADDR):
            self.bus = Bus(bus_num)
            self.addr = addr

    #Fetch data from sensor in mV and convert it into A

    def get_nchan_vol_milli_data(self,n,ring_buffer):
        summation = 0
        intdata = 0
        num_of_summed_data = 0
        previous_reading = 0
        for i in range (int(ring_buffer)):
            data = self.bus.read_i2c_block_data(0X04, 0X23, 2)         # 0X23 in pin number 3 i.e CT03 
            if (data[1]<<8|data[0]) > 0:
                intdata += data[1]<<8|data[0]                          #convert output in the form of list to interger by using bitwise OR operation
                num_of_summed_data += 1
                #print (intdata)
                #print (num_of_summed_data)
        Vo = intdata / num_of_summed_data 
        #print (Vo)
        mV = ((Vo * 3300 ) / 4096 )
        #print (mV)
        mV= mV - 3                                                       # output voltage when no load =3 so offset it
        #print(mV)
        ipri = ((mV / 60 ) * 3)                                          #current in ampere 
        summation = ( ipri ** 2 )
        #print (summation)
        irms = (math.sqrt (summation )) * 0.707                          #RMS value of current in Ampere
        apperentpower = 210 * irms                                       # power in watt Vrms * Irms
        print ("VA:" + str(apperentpower ))
        return irms

ADC = Current()
def main():
    #pin = 3
    sampling= 1000 
    ring_buffer = sampling * 1.5 
    while True:
        pin_current = ADC.get_nchan_vol_milli_data( 0X23 ,ring_buffer)
        print("pin_current(A):" + str(pin_current))
        time.sleep(5)

if __name__ == '__main__':
    main()

    

