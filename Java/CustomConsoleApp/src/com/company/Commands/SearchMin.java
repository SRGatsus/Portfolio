package com.company.Commands;

import com.company.CustomConsoleLineModule.Command;
import com.company.SqrtDecompositions.SqrtDecompos;
import com.company.InputSystem.CustomInputSystim;
/**
 *@author Roman Korotaev
 *@since 1.0
 *Implements the search for the minimum number in the interval
 */
public class SearchMin implements Command {
    public SqrtDecompos SqrtDecompos=null;

    /**
     *{@inheritDoc}
     * which implements the search for the minimum number by the SQRT decomposition method on the interval. There must be two arguments.
     *      If there is one argument, then you will be asked to enter a second argument.
     *      If one of the arguments is greater than the length of the array or less than zero, an error will be displayed and a request will be made to correct this argument.
     * There is also an optional 3 argument, responsible for finding the minimum number in the usual way.
     *      If it exists, it must be True.
     * @param args array of arguments
     * @return returns a boolean value equal to true
     */
    @Override
    public boolean execute(String... args) {
        if(args==null||SqrtDecompos==null){
            System.err.println("Error");
            return true;
        }
        int n1,n2=0;
        try {
            n1=Integer.parseInt(args[0]);
            if (args.length!=1){
                n2=Integer.parseInt(args[1]);
            }
        }catch (NumberFormatException e){
            System.err.println("Input error");
            return true;
        }
        n1=CustomInputSystim.CheckNumber(n1,0,SqrtDecompos.GetLengArray()-1,"Enter the number","The left border is entered incorrectly");
        n2=CustomInputSystim.CheckNumber(n2,0,SqrtDecompos.GetLengArray()-1,"Enter the number","The right border was entered incorrectly");
        int left=Math.min(n1,n2);
        int right=Math.max(n1,n2);
        System.out.println("Minimum number via SQRT decompression: "+SqrtDecompos.SearchMinSQRT(left,right));
        if(args.length>2&&args[2].toUpperCase()=="TRUE"){
            SqrtDecompos.SearchMinStandart(left,right);
            System.out.println("Minimum number via normal search "+SqrtDecompos.SearchMinStandart(left,right));
        }
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
        return "SEARCHMIN";
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public String getDescription() {
        return "This command searches for the minimum number in the interval using the SQRT decompression method. There must be two parameters left and right border. Also, the third parameter is optional, the true flag. Outputs a number search using the usual method";
    }

}
