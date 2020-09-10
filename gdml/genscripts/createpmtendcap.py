from __future__ import print_function
import ROOT as root
import math
import os
import sys

#All units in cm
total_endcap = 250.0 #full 2.5 m radius
fiducial_rad = 225.0 #2.5 m - 25 cm standoff
pmt_rad = 4.0*2.54 #8" PMT spaced at least 1/2" apart
pmtdiameter = 8.0*2.54
pmtplussd = (8.0*2.54 + 0.5*2.54)
pmt_height = 220.0 #leave 30 cm from ends to allow for PMT placement in reality, R5912 PMTs are max 275 cm (leaves room for cables, etc. at either end
tpb_height = 220.0 - (pmt_rad+0.0001)

vis_hist = root.TH2D("vis_hist", "Placement of PMTs in top endcap;x (cm);y (cm)",200,-total_endcap,total_endcap, 200, -total_endcap, total_endcap) 

can = root.TCanvas()
fullcap = root.TEllipse(0.0,0.0,total_endcap, total_endcap)
fidcap = root.TEllipse(0.0,0.0,fiducial_rad,fiducial_rad)
can.Draw()
vis_hist.SetStats(0)
vis_hist.Draw()
fullcap.Draw()
fidcap.Draw("lsame")
can.Update()
#can.Print("test.pdf")

pmt_pos = []
pmt_circles = []

max_pmt = int((fiducial_rad - 0.5*pmtplussd)/pmtplussd)
print(max_pmt)

x_center = 0
y_center = 0

for i in range(max_pmt+1):
    for j in range(max_pmt+1):
        x = i*pmtplussd
        y = j*pmtplussd
        rad = math.sqrt(x*x + y*y)
        if(rad+pmt_rad > fiducial_rad):
            continue
        pmt = root.TEllipse(x,y,pmt_rad, pmt_rad)
        pmt_circles.append(pmt)
        pmt_pos.append([x,y,pmt_height])
        pmt.Draw("lsame")
        if(x > 0):
            pmt = root.TEllipse(-x,y,pmt_rad,pmt_rad)
            pmt_pos.append([-x,y,pmt_height])
            pmt_circles.append(pmt)
            pmt.Draw("lsame")
        if(y > 0):
            pmt = root.TEllipse(x,-y,pmt_rad,pmt_rad)
            pmt_pos.append([x,-y,pmt_height])
            pmt_circles.append(pmt)
            pmt.Draw("lsame")
        if(x > 0 and y > 0):
            pmt = root.TEllipse(-x,-y,pmt_rad,pmt_rad)
            pmt_pos.append([-x,-y,pmt_height])
            pmt_circles.append(pmt)
            pmt.Draw("lsame")
for i in range(max_pmt+1):
    for j in range(max_pmt+1):
        x = i*pmtplussd
        y = j*pmtplussd
        rad = math.sqrt(x*x+y*y)
        if(rad+pmt_rad > fiducial_rad):
            continue
        pmt = root.TEllipse(x,y,pmt_rad, pmt_rad)
        pmt_pos.append([x,y,-pmt_height])
        pmt = root.TEllipse(-x,y,pmt_rad,pmt_rad)
        pmt_pos.append([-x,y,-pmt_height])
        pmt = root.TEllipse(x,-y,pmt_rad,pmt_rad)
        pmt_pos.append([x,-y,-pmt_height])
        pmt = root.TEllipse(-x,-y,pmt_rad,pmt_rad)
        pmt_pos.append([-x,-y,-pmt_height])
        
can.Update()
can.Print("pmtplacetest.pdf")

print("Total PMTs / endcap ", len(pmt_circles))

#create gdml backends for endcap with holes and pmt positions

pos_out = open('../simple_target_detector/pmt_endcap_positions.xml', 'w')
place_out = open('../simple_target_detector/pmt_placement.xml', 'w')
pos_tpb_out = open('../simple_target_detector/tpb_endcap_positions.xml', 'w')
rot_tpb_out = open('../simple_target_detector/tpb_endcap_rotations.xml', 'w')
tpb_out = open('../simple_target_detector/tpb_placement.xml', 'w')
endcap_out = open('../simple_target_detector/endcap_subsolid.xml', 'w')
endcap_tpb_out = open('../simple_target_detector/endcap_tpb_subsolid.xml', 'w')
hole_out = open('../simple_target_detector/endcap_pmthole_positions.xml', 'w')

for i,pmt in enumerate(pmt_circles):
    print("<position name=\"PMT_pos%d\" unit=\"cm\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" />"%(i,pmt.GetX1(), pmt.GetY1(), pmt_height), file=pos_out)
    print("<position name=\"PMT_pos%d\" unit=\"cm\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" />"%(i+len(pmt_circles),pmt.GetX1(), pmt.GetY1(), -pmt_height), file=pos_out)

    print("<rotation name=\"TPB_rot%d\" unit=\"deg\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" />"%(i,0.0,0.0,180.0), file = rot_tpb_out)
    print("<rotation name=\"TPB_rot%d\" unit=\"deg\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" />"%(i+len(pmt_circles),0.0,0.0,180.0), file = rot_tpb_out)

    print("<position name=\"TPB_pos%d\" unit=\"cm\" x=\"%.4f\" y=\"%.4f\" z=\"%.4f\" />"%(i, pmt.GetX1(), pmt.GetY1(), pmt_height), file=pos_tpb_out)
    print("<position name=\"TPB_pos%d\" unit=\"cm\" x=\"%.4f\" y=\"%.4f\" z=\"%.4f\" />"%(i+len(pmt_circles), pmt.GetX1(), pmt.GetY1(), -pmt_height), file=pos_tpb_out)
    
    print("<physvol name=\"topcapPMT%d\" copynumber=\"%d\">"%(i,i), file = place_out)
    print("  <volumeref ref=\"PMTLog\" />", file = place_out)
    print("  <positionref ref=\"PMT_pos%d\"/>"%(i), file = place_out)
    print("</physvol>", file = place_out)
    
    print("<physvol name=\"bottomcapPMT%d\" copynumber=\"%d\">"%(i+len(pmt_circles),i+len(pmt_circles)), file = place_out)
    print("  <volumeref ref=\"PMTLog\" />", file = place_out)
    print("  <positionref ref=\"PMT_pos%d\"/>"%(i+len(pmt_circles)), file = place_out)
    print("</physvol>",file = place_out)

    print("<physvol name=\"topcapTPB%d\" copynumber=\"%d\">"%(i,i), file = tpb_out)
    print("  <volumeref ref=\"TPBTopLog\" />", file = tpb_out)
    print("  <positionref ref=\"TPB_pos%d\"/>"%(i), file = tpb_out)
    print("  <rotationref ref=\"TPB_rot%d\"/>"%(i), file = tpb_out)
    print("</physvol>", file = tpb_out)

    print("<physvol name=\"bottomcapTPB%d\" copynumber=\"%d\">"%(i+len(pmt_circles),i+len(pmt_circles)), file = tpb_out)
    print("  <volumeref ref=\"TPBBotLog\" />", file = tpb_out)
    print("  <positionref ref=\"TPB_pos%d\"/>"%(i+len(pmt_circles)), file = tpb_out)
    print("  <rotationref ref=\"TPB_rot%d\"/>"%(i+len(pmt_circles)), file = tpb_out)
    print("</physvol>",file = tpb_out)
    
    print("<position name=\"PMTHole_pos%d\" unit=\"cm\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" />"%(i,pmt.GetX1(), pmt.GetY1(), 0.0), file=hole_out)


for i in range(len(pmt_circles)):
    if(i+1 < len(pmt_circles)):
        print("<subtraction name=\"%s%d\">"%("TPBCapWithPMTHole",i+1), file = endcap_tpb_out)
    else:
        print("<subtraction name=\"%s\">"%("TPBEndCap"), file = endcap_tpb_out)
    print("  <first ref=\"%s%d\" />"%("TPBCapWithPMTHole", i), file = endcap_tpb_out)
    print("  <second ref=\"%s\" />"%("PMTHole"), file = endcap_tpb_out)
    print("  <positionref ref=\"PMTHole_pos%d\" />"%(i), file = endcap_tpb_out)
    print("</subtraction>", file=endcap_tpb_out)

for i in range(len(pmt_circles)):
    if(i+1 < len(pmt_circles)):
        print("<subtraction name=\"%s%d\">"%("CapWithPMTHole",i+1), file = endcap_out)
    else:
        print("<subtraction name=\"%s\">"%("EndCap"), file = endcap_out)
    print("  <first ref=\"%s%d\" />"%("CapWithPMTHole", i), file = endcap_out)
    print("  <second ref=\"%s\" />"%("PMTHole"), file = endcap_out)
    print("  <positionref ref=\"PMTHole_pos%d\" />"%(i), file = endcap_out)
    print("</subtraction>", file=endcap_out)

pos_out.close()
place_out.close()
hole_out.close()
endcap_out.close()
endcap_tpb_out.close()
