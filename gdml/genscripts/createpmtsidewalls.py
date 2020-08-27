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

print(num_pmt_levels) 

pmt_positions = []
pmt_rotations = []
pmt_circles = []
#Place PMTs
pos = 0
num_pmt = 0
num_pmt_side = 0
for num in range(num_pmt_levels/2):
    angle = 15
    z = num*pmtplussd;
    for deg in range(0, 360, angle):
        phi = deg*(2.0*math.pi/360.0)
        x = fiducial_rad*math.cos(phi)
        y = fiducial_rad*math.sin(phi)
        u = math.cos(math.pi - phi)
        v = math.cos(math.pi/2.0 + phi)
        w = math.cos(math.pi/2.0)
        pmt_positions.append([x,y,z])
        pmt_rotations.append([u,v,w])
        num_pmt_side = num_pmt_side + 1
for num in range(1,num_pmt_levels/2):
    angle = 15
    z = num_pmt_levels*pmtplussd;
    for deg in range(0, 360, angle):
        phi = deg*(2.0*math.pi/360.0)
        x = pmt_rad*math.cos(phi)
        y = pmt_rad*math.sin(phi)
        u = math.cos(math.pi - phi)
        v = math.cos(math.pi/2.0 + phi)
        w = math.cos(math.pi/2.0)
        pmt_positions.append([x,y,-z])
        pmt_rotations.append([u,v,w])
        num_pmt = num_pmt + 1

side_pos_out = open('../simple_target_detector/pmt_sidewall_positions.xml', 'w')
side_place_out = open('../simple_target_detector/pmt_sidewall_placements.xml', 'w')
#for rotations, need <rotation name="" x="" y="" z="" unit="deg,rad />
#to place rotated PMT, need to include <rotationref ref="" /> as part of physvol placement block
side_rot_out = open('../simple_target_detector/pmt_sidewall_rotations.xml', 'w')
print(pmt_positions[1][0])
for i in range(num_pmt_side):
    print("<position name=\"side_pmt_pos%d\" unit=\"cm\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" />"%(i, pmt_positions[i][0],pmt_positions[i][1], pmt_positions[i][2]), file=side_pos_out)
    if(pmt_positions[i][2] > 0):
        print("<position name=\"side_pmt_pos%d\" unit=\"cm\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" />"%(i+num_pmt, pmt_positions[i][0],pmt_positions[i][1], -pmt_positions[i][2]), file=side_pos_out)
    print("<rotation name=\"side_pmt_rot%d\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" unit=\"deg\" />"%(i, pmt_rotations[i][0]*(180/math.pi), pmt_rotations[i][1]*(180/math.pi), pmt_rotations[i][2]*(180/math.pi)), file=side_rot_out)
    if(pmt_positions[i][2] > 0):
         print("<rotation name=\"side_pmt_rot%d\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" unit=\"deg\" />"%(i+num_pmt, pmt_rotations[i][0]*(180/math.pi), pmt_rotations[i][1]*(180/math.pi), pmt_rotations[i][2]*(180/math.pi)), file=side_rot_out)
    print("<physvol name=\"sidePMT%d\" copynumber=\"%d\">"%(i,i), file=side_place_out)
    print("  <volumeref ref=\"PMTLog\" />", file=side_place_out)
    print("  <positionref ref=\"side_pmt_pos%d\"/>"%(i), file=side_place_out)
    print("  <rotationref ref=\"side_pmt_rot%d\"/>"%(i), file=side_place_out)
    print("</physvol>", file=side_place_out)
    if(pmt_positions[i][2] > 0):
        print("<physvol name=\"sidePMT%d\" copynumber=\"%d\">"%(i+num_pmt,i+num_pmt), file=side_place_out)
        print("  <volumeref ref=\"PMTLog\" />", file=side_place_out)
        print("  <positionref ref=\"side_pmt_pos%d\"/>"%(i+num_pmt), file=side_place_out)
        print("  <rotationref ref=\"side_pmt_rot%d\"/>"%(i+num_pmt), file=side_place_out)
        print("</physvol>", file=side_place_out)

side_pos_out.close()
side_rot_out.close()
side_place_out.close()
 

    
    

