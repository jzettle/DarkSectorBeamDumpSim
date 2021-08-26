

filename = "/darksec/app/users/jzettle/BdNMC/BdNMC/Events/Fermilab_dm_50mev_Updated.dat"
outfile = "splitDMResults_ArRecoils.txt"
outfiledm = "splitDMResults_DM.txt"
out = open(outfile, 'w')
outdm = open(outfiledm, 'w')
values = []
count = 0
with open(filename) as f:
    for line in f:
        values.append([n for n in line.strip().split(" ")])
        if(len(values[count]) > 8 and values[count][0] == 'Liquid_Argon'):
            outstring = ''
            for i in range(1, len(values[count])):
                if(i < len(values[count])-1):
                   outstring = outstring + str(values[count][i]) + ' ' 
                if(i == len(values[count])-1):
                   outstring = outstring + str(values[count][i]) + '\n'
            out.write(outstring)
        if(len(values[count]) > 8 and values[count][0] == 'DM'):
            outstring = ''
            for i in range(1, len(values[count])):
                if(i < len(values[count])-1):
                    outstring = outstring + str(values[count][i]) + ' '
                if(i == len(values[count])-1):
                    outstring = outstring + str(values[count][i]) + '\n'
            outdm.write(outstring)
        count = count + 1              
f.close()
out.close()    
outdm.close()
        
