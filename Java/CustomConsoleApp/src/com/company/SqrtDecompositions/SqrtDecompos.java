package com.company.SqrtDecompositions;
import com.company.InputSystem.CustomInputSystim;


public class SqrtDecompos {

    /**
     * Error variable if the array is not created
     */
    private static final String ARRAY_NOT_CREATE = "Array not create";

    /**
     * Error variable if converted to a number
     */
    private static final String ERROR_CONVERT = "Error in converting a string to a number.Try again.";

    /**
     * Variable interval length
     */
    private int IntervalLength=0;

    /**
     * Array of all numbers
     */
    private double[] ArrayNumbers=null;

    /**
     * Array of minimum numbers from each interval
     */
    private double[] ArrayMinNumbers=null;

    /**
     * The function initializes an array of numbers with length N and an array of minimal numbers, and also calculates the length of the interval.
     * @param N length array
     */

    public void Init(int N){
        IntervalLength=(int)Math.sqrt(N);
        ArrayNumbers=new double[N];
        ArrayMinNumbers=new double[((int)Math.ceil((float)N/(float)IntervalLength))];
        for (int i=0;i<ArrayMinNumbers.length;i++){
            ArrayMinNumbers[i]=Double.MAX_VALUE;
        }
    }

    /**
     * Deletes an array
     */
    public void Delete(){
        ArrayNumbers=null;
        IntervalLength=0;
    }

    /**
     * @return an array number
     */
    public double[] getArrayNumbers() {
        return ArrayNumbers;
    }

    /**
     * Checks whether an array has been created
     * @return true-array created, false-not created
     */
    public boolean IsArray(){
        return ArrayNumbers==null?false:true;
    }

    /**
     * Returns the length of the array
     * @return array length
     */
    public int GetLengArray(){return ArrayNumbers.length;}

    /**
     * Implements array filling. If the array is not created, it throws an error
     */
    public void Fill(){
        if(ArrayNumbers!=null){
            CustomInputSystim CIS=new CustomInputSystim();
            for (int index=0;index<ArrayNumbers.length;index++){
                ArrayNumbers[index]=CIS.DoubleInput("Введите элемент №"+index+" :",ERROR_CONVERT);
            }
            FillMinArray();
        }else{
            System.err.println(ARRAY_NOT_CREATE);
        }
    }

    /**
     * Implements filling an array at a point by index. If the index is out of range, an error is displayed.
     * @param arg the argument to be inserted into the dot
     * @param index the point itself where the insertion will be
     */
    public void Input(int index,double arg){
        if(ArrayNumbers!=null){
            if(index>ArrayNumbers.length ||index<0){
                System.err.println("Invalid index was output");
                return;
            }
            ArrayNumbers[index]=arg;
            FillMinArray();
        }else{
            System.err.println(ARRAY_NOT_CREATE);
        }
    }

    /**
     * Output of a certain value by index.
     * @param index  index of the value in the array
     */
    public double Output(int index){
        if(ArrayNumbers!=null){
            return ArrayNumbers[index];
        }else{
            System.err.println(ARRAY_NOT_CREATE);
            return 0;
        }
    }

    /**
     * Finding the minimum value on the interval via SQRT decomposition
     * @param left left border
     * @param right right border
     */
    public double SearchMinSQRT(int left, int right){
        double min=Double.MAX_VALUE;
        if(ArrayNumbers!=null){
            int minBL=(int)left/IntervalLength;
            int minBR=(int)right/IntervalLength;
            if(minBL==minBR){
                for (int i=left;i<=right;i++){
                    min=Math.min(min,ArrayNumbers[i]);
                }
            }else{
                for (int i=minBL+1;i<=minBR-1;i++){
                    min=Math.min(min,ArrayMinNumbers[i]);
                }
                for (int i=left;i<=(minBL+1)*IntervalLength;i++){
                    min=Math.min(min,ArrayNumbers[i]);
                }
                for (int i=right;i>=(minBR)*IntervalLength;i--){
                    min=Math.min(min,ArrayNumbers[i]);
                }
            }
        }else{
            System.err.println(ARRAY_NOT_CREATE);
        }
        return min;
    }

    /**
     * Finding the minimum value in the interval through the bubble
     * @param left left border
     * @param right right border
     */
    public double SearchMinStandart(int left, int right){
        double min=Double.MAX_VALUE;
        if(ArrayNumbers!=null){
            for (int i=left;i<=right;i++){
                min=Math.min(ArrayNumbers[i],min);
            }
        }else{
            System.err.println(ARRAY_NOT_CREATE);
        }
        return min;
    }

    /**
     * Filling an array with random numbers
     * @param max maximum allowed value
     * @param min minimum allowed value
     */
    public void FillRandom(int min,int max){
        if(ArrayNumbers!=null){
            for (int index=0;index<ArrayNumbers.length;index++){
                ArrayNumbers[index]=Math.round((Math.random()*((max-min)+1))+min);
            }
            FillMinArray();
        }else{
            System.err.println(ARRAY_NOT_CREATE);
        }
    }

    /**
     *Filling of the minimum array, filling only occurs if there have been changes in the array or the array has just been filled
     */
    public void FillMinArray(){
        if(ArrayNumbers!=null){
            for (int index=0;index<ArrayNumbers.length;index++){
                int indexMin=(int)index/IntervalLength;
                //System.out.println("Index"+indexMin+" index:"+index+" длинна:"+ IntervalLength);
                ArrayMinNumbers[indexMin]=Math.min(ArrayNumbers[index],ArrayMinNumbers[indexMin]);
            }
        }else{
            System.err.println(ARRAY_NOT_CREATE);
        }
    }

    /**
     * Displaying an array on the screen
     */
    public void PrintMain(){
        if(ArrayNumbers!=null){
            for (int index = 0; index < ArrayNumbers.length; index++){
                System.out.println("Элемент № "+(index)+" : "+ArrayNumbers[index]);
            }
        }else{
            System.err.println(ARRAY_NOT_CREATE);
        }

    }

    /**
     * Output of an array to the screen with the caption in which interval each number is located
     */
    public void PrintMainInterval(){
        if(ArrayNumbers!=null){
            for (int index = 0; index < ArrayNumbers.length; index++){
                int indexMin=(int)index/IntervalLength;
                System.out.println("Элемент №"+(index)+" находиться в интервале №"+indexMin+" : "+ArrayNumbers[index]);
            }
        }else{
            System.err.println(ARRAY_NOT_CREATE);
        }

    }

    /**
     * Output of the minimum array
     */
    public void PrintMinimumArray(){
        if(ArrayNumbers!=null){
            for (int index = 0; index < ArrayMinNumbers.length; index++){
                System.out.println("Элемент №"+(index)+" : "+ArrayMinNumbers[index]);
            }
        }else{
            System.err.println(ARRAY_NOT_CREATE);
        }

    }
}
