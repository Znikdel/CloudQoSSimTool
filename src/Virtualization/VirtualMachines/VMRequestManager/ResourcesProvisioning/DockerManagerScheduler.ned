//
// Module that is the core of a cloud manager scheduler
//
// The vmImages are the different virtual machines that the scheduler offers to the users
//
// @authors Gabriel Gonz&aacute;lez Casta&ntilde;&eacute
// @date 2014-12-11
// disabled print energy part by default value to false
//@author bing gao
//@date 2016-04-14

package icancloud.src.Virtualization.VirtualMachines.VMRequestManager.ResourcesProvisioning;

import icancloud.src.Virtualization.VirtualMachines.VMRequestManager.ResourcesProvisioning.IDockerScheduler;
import icancloud.src.Virtualization.VirtualMachines.Docker.IDocker;

module DockerManagerScheduler like IDockerScheduler
{
    parameters:
        
        // The virtual ip that the cloud will give to the VMs. It will be increased by 1 each new VM.
		string virtualIPsBasis = default ("10.0.0.1");					      
     
     	// The time that cloud waits until it starts
		double timeToStart = default(0.0); // Time to start cloud manager
       
        // The minimum quantity of data before disconnect the VM and reconnect it.
        int dirtyingRate = default(256);
        
        // The number of parallel servers used as remote storage
        int numberOfPFSRemoteServers;
        
        // Time until the scheduler is activated again 
        int timeBetweenScheduleEvents_s = default(1);
        
        // Time between all data center is checked extracting the energy measurements.
        double timeBetweenLogResults_s = default(1.0);
        
        // print the energy to a log file.
        bool printEnergyToFile = default(false);     
        
  
        // The maximum number of virtual machines into a node
        int numberOfDockerperVM = default(10);
        
        // Parameter pending to test. Do not change the value
       // bool memorization = default (false);
        
        // .ned Name for the VMs that will be the images that user virtual machines will replicate.
        string vmNedImageName = default ("DockerContainer");
        
        // compress the output files
     //   bool outputCompression = default (false);
        
        @class(VMRequestManager);
        @display("i=msg/job");

	submodules:
	    dockerImage:<vmNedImageName> like IDocker {
	        numCores = 0;
	        memorySize_MB = 0;
	        storageSize_GB = 0;
	        @display("i=old/telnet");
	    }
}
