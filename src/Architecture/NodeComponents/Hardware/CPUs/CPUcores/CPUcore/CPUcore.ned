// Module that implements a Generic CPUcore.
//
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30
//
// @author Gabriel Gonz&aacute;lez Casta;&ntilde;&eacute
// @date 2013-10-27


package icancloud.src.Architecture.NodeComponents.Hardware.CPUs.CPUcores.CPUcore;

import icancloud.src.Architecture.NodeComponents.Hardware.CPUs.CPUcores.ICPUcore;

simple CPUcore like ICPUcore

{
    parameters:
        double speed;			    // CPU core speed (in MIPS)
        double tick_s;				// Maximum tick time (in useconds)

    gates:
        input in;		// Input gate
        output out;		// Output gate

}
