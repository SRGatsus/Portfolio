package com.company.Commands;

import com.company.CustomConsoleLineModule.Command;

import static com.company.InputSystem.CustomInputSystim.CheckNumber;
import com.company.SqrtDecompositions.SqrtDecompos;
import com.company.InputSystem.CustomInputSystim;
/**
 *@author Roman Korotaev
 *@since 1.0
 * Implements array initialization
 */
public class Initialization implements Command {
    public SqrtDecompos SqrtDecompos=null;

    /**
     *{@inheritDoc}
     * which implements array initialization. There can be one parameter that must be an integer.
     *      If it is not there, the function will ask you to enter it.
     * @param args array of arguments
     * @return returns a boolean value equal to true
     */
    @Override
    public boolean execute(String... args) {
        if(SqrtDecompos==null){
            System.err.println("Error");
            return true;
        }
        int N=0;
        if(args==null){
            N= CheckNumber(CustomInputSystim.IntInput("Enter the number of elements in the array","You entered incorrectly") ,1,2000,"Enter the number of elements in the array","You entered incorrectly");
            if(SqrtDecompos.IsArray()){
                SqrtDecompos.Delete();
            }
            SqrtDecompos.Init(N);
            return true;
        }
        N= CheckNumber(Integer.parseInt(args[0])  ,1,2000,"Enter the number of elements in the array","You entered incorrectly");
        if(SqrtDecompos.IsArray()){
            SqrtDecompos.Delete();
        }
        SqrtDecompos.Init(N);
        return true;
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public void printHelp() {
        System.out.println(getDescription());
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public String getName() {
        return "INIT";
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public String getDescription() {
        return "This command implements array initialization. The only optional parameter is the length of the array. If you do not lead, the program itself will ask you to enter the length.";
    }

}
