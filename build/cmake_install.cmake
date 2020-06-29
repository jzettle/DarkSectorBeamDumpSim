# Install script for directory: /nashome/j/jzettle/DarkSectorBeamDumpSim

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/darksectorsim" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/darksectorsim")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/darksectorsim"
         RPATH "/cvmfs/fermilab.opensciencegrid.org/products/larsoft/root/v6_18_04/Linux64bit+2.6-2.12-e17-prof/lib:/cvmfs/fermilab.opensciencegrid.org/products/larsoft/geant4/v4_10_6_p01/Linux64bit+2.6-2.12-e19-prof/lib64:/cvmfs/fermilab.opensciencegrid.org/products/larsoft/xerces_c/v3_2_2/Linux64bit+2.6-2.12-e19-prof/lib:/cvmfs/fermilab.opensciencegrid.org/products/larsoft/clhep/v2_4_1_2/Linux64bit+2.6-2.12-e19-prof/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/nashome/j/jzettle/DarkSectorBeamDumpSim/build/darksectorsim")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/darksectorsim" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/darksectorsim")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/darksectorsim"
         OLD_RPATH "/cvmfs/fermilab.opensciencegrid.org/products/larsoft/root/v6_18_04/Linux64bit+2.6-2.12-e17-prof/lib:/cvmfs/fermilab.opensciencegrid.org/products/larsoft/geant4/v4_10_6_p01/Linux64bit+2.6-2.12-e19-prof/lib64:/cvmfs/fermilab.opensciencegrid.org/products/larsoft/xerces_c/v3_2_2/Linux64bit+2.6-2.12-e19-prof/lib:/cvmfs/fermilab.opensciencegrid.org/products/larsoft/clhep/v2_4_1_2/Linux64bit+2.6-2.12-e19-prof/lib:"
         NEW_RPATH "/cvmfs/fermilab.opensciencegrid.org/products/larsoft/root/v6_18_04/Linux64bit+2.6-2.12-e17-prof/lib:/cvmfs/fermilab.opensciencegrid.org/products/larsoft/geant4/v4_10_6_p01/Linux64bit+2.6-2.12-e19-prof/lib64:/cvmfs/fermilab.opensciencegrid.org/products/larsoft/xerces_c/v3_2_2/Linux64bit+2.6-2.12-e19-prof/lib:/cvmfs/fermilab.opensciencegrid.org/products/larsoft/clhep/v2_4_1_2/Linux64bit+2.6-2.12-e19-prof/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/darksectorsim")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/nashome/j/jzettle/DarkSectorBeamDumpSim/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
