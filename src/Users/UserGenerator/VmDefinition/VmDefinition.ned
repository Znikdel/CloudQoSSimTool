//
// Module that implements a virtual machine definition that tenants can rent.
//
// @author Gabriel Gonzalez Casta;&ntilde;e
// @date 2012-11-30

package icancloud.src.Users.UserGenerator.VmDefinition;

import icancloud.src.Users.UserGenerator.VmDefinition.VMToRent.VmToRent;

module VmDefinition
{
    parameters:
        int vmsToRentTypesQuantity;							// Quantity of vms that will be rented

    submodules:
        vmToRent[vmsToRentTypesQuantity]: VmToRent {
            @display("p=133,56");
        }
}
