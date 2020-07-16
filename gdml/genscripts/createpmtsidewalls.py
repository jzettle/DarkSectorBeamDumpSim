from __future__ import print_function
import ROOT as root
import math
import os
import sys

fiducial_rad = 225 # 2.5 m - 25 cm standoff
pmt_rad = 4.0*2.54
pmt_diameter = 8.0*2.54
pmtplussd = (8.0*2.54 + 0.5*2.54) #at least 1/2" spacing between any PMT
total_height = 450 # 5 m height with 25 cm standoff on each end 

num_pmt_levels = int((total_height - 0.5*pmtplussd)/pmtplussd)
rad_pmt = (2.0*math.pi)/pmt_diameter
num_pmts = int((2.0*math.pi/rad_pmt))

print(num_pmt_levels) 
print("Total number of PMTs per level: ", num_pmts)

pmt_dx = fiducial_rad*math.cos(0.5*rad_pmt) - pmtplussd
pmt_dy = fiducial_rad*math.sin(0.5*rad_pmt) - pmtplussd

pmt_positions = []
pmt_rotations = []

#Rotations for each pmt in radians
for num in range(num_pmts):
    pmt_rotations.append((num+0.5)*rad_pmt)
print(pmt_rotations)
for i in range(num_pmt_levels):
    

