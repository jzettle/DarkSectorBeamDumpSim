#!/usr/bin/env python
##################################################
# ppfx grid job submitter
# 2019.07 -- L. Aliaga
##################################################
import os, optparse, random, shutil, tarfile, sys
import subprocess, string

CACHE_PNFS_AREA = "/pnfs/ldrd/darksectorldrd/scratch/users/{USER}/grid_cache/".format(USER = os.getenv("USER"))
PWD = os.getenv("PWD")

##################################################
# Job Defaults
##################################################
N_JOBS             = 1000
OUTDIR             = "/pnfs/ldrd/darksectorldrd/persistent/users/{USER}/darksectorsim/Electrons_".format(USER = os.getenv("USER")) + str(random.randint(0,10000)) + "/"
#INPUT_OPTIONS      = "{0}/scripts/inputs_default.xml".format(PWD)
#INPUT_OPTIONS      = "scripts/inputs_default.xml"
#IDET               = "3"
#INDIR              = "/pnfs/nova/data/flux/g4numi/v6r1"
TARFILE_NAME       = "darksectorsim.tar.gz"
#BEAMCONFIG         = "me000z200i"

##################################################

def main():
  options = get_options()
  
  cache_folder = CACHE_PNFS_AREA + str(random.randint(10000,99999)) + "/"
  os.makedirs(cache_folder)
  
  os.makedirs(options.outdir)

  print "\nTarring up local area..."
  make_tarfile(TARFILE_NAME, ".")

  shutil.move(TARFILE_NAME,    cache_folder) 
  shutil.copy("electrons.sh", cache_folder)
  
  print "\nTarball of local area:", cache_folder + TARFILE_NAME

  logfile = options.outdir + "darksectorsim_\$PROCESS.log"
  
  print "\nOutput logfile(s):",logfile

  #Add {MEMORY} if bring back memory command
  submit_command = ("jobsub_submit {GRID} -N {NJOBS} " 
      "-G darksectorldrd "   
      "-d PPFX {OUTDIR} " 
      "-f {TARFILE} "
      "-L {LOGFILE} "
      "file://{CACHE}/electrons.sh".format(
      GRID       = ("--OS=SL7 -g "
                    "--resource-provides=usage_model=OPPORTUNISTIC,OFFSITE "
                    "--role=Analysis "
                    "--expected-lifetime='long' "),
                    #"--append_condor_requirements='(TARGET.HAS_SINGULARITY=?=true)' -l '+SingularityImage="
                    #"/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-wn-sl7:latest'"),
      #MEMORY     = "--memory 200MB ",
      NJOBS      = options.n_jobs,
      OUTDIR     = options.outdir,
      TARFILE    = cache_folder + TARFILE_NAME,
      LOGFILE    = logfile,
      CACHE      = cache_folder)
  )

  #Ship it
  print "\nSubmitting to grid:\n"+submit_command+"\n"
  status = subprocess.call(submit_command, shell=True)

def get_options():
  parser       = optparse.OptionParser(usage="usage: %prog [options]")
  grid_group   = optparse.OptionGroup(parser, "Grid Options")
  
  grid_group.add_option("--outdir",
                        default = OUTDIR,
                        help    = "Output flux histograms location. Default = %default.")
  
  grid_group.add_option("--n_jobs",
                        default = N_JOBS,
                        help = "Number of g4numi jobs. Default = %default.")
  
  parser.add_option_group(grid_group)

  options, remainder = parser.parse_args()

  return options

def make_tarfile(output_filename, source_dir):
  tar = tarfile.open(output_filename, "w:gz")
  for i in os.listdir(source_dir):
    tar.add(i)
  tar.close()

if __name__ == "__main__":
  sys.exit(main())
