package com.company.Commands;

import com.company.CustomConsoleLineModule.Command;
import com.company.SqrtDecompositions.SqrtDecompos;
import com.company.InputSystem.CustomInputSystim;
/**
 *@author Roman Korotaev
 *@since 1.0
 * Implements array output
 */
public class Print implements Command {

    public SqrtDecompos SqrtDecompos=null;

    /**
     *{@inheritDoc}
     * Which implements array output. There are three states depending on the first argument:
     *      If there are no arguments or it is equal to 0, then the full array is output.
     *      If the argument is equal to 1, then the minimum array is output (the minimum numbers from each range).
     *      If the argument is 2, then an array is output with the signature in which range each element is located.
     * @param args array of arguments
     * @return returns a boolean value equal to true
     */
    @Override
    public boolean execute(String... args) {
        if(SqrtDecompos==null){
            System.err.println("Error");
            return true;
        }
        try {
            if(args==null||Integer.parseInt(args[0])==0){
                SqrtDecompos.PrintMain();
            }else if(Integer.parseInt(args[0])==1){
                SqrtDecompos.PrintMainInterval();

            }else if(Integer.parseInt(args[0])==2){
                SqrtDecompos.PrintMinimumArray();
            }else{
                System.err.println("Invalid command format"+args[0]);
            }
            return true;
        }catch (NumberFormatException e){

        }
        System.err.println("Invalid command format"+args[0]);
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
        return "PRINT";
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public String getDescription() {
        return "Displays an array on the screen. There is one parameter that is optional, but if there is, it must be in the range from 0 to 2 inclusive.\n 0 is the output of the array itself.\n 1-output of the minimum array.\n 2-output of an array with the signature which digit is in which interval";
    }
}
