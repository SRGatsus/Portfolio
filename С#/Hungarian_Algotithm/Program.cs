using System;

namespace Hungarian_Algotithm
{
    class Program
    {
        static int P = 13;
        static void GetNumber(string value, out int Number)
        {
            bool isNumber = false;
            string stringNumber = "";
            int Number2 = 0;

            while (!isNumber)
            {
                Console.WriteLine(value);
                bool r;
                stringNumber = Console.ReadLine();

                r = int.TryParse(stringNumber, out Number2);

                if (r)
                {
                    isNumber = true;
                }
                else
                {
                    Console.WriteLine("Error:Вы ввели не число(либо не целое)");
                }
            }
            Number = Number2;
        }
        static void OutputData(int[,] Matrix)
        {
            for (int i = 0; i < Matrix.GetLength(0); i++)
            {
                for (int j = 0; j < Matrix.GetLength(1); j++)
                {
                    Console.Write(Matrix[i, j] + "\t");
                }
                Console.WriteLine();
            }
        }

        static void CalcMetod(int[,] Matrix, int[,] InputData, int StartOperation, int Duration)
        {
            int IndexX = 1;
            int IndexY = 1;
            int CountX = Matrix.GetLength(0);
            int CountY = Matrix.GetLength(1);
            int f1 = 0;
            bool flag = false;
            while (CountX >=IndexX)
            {
                flag = true;
                IndexY = IndexX;
                int a = 0;
                while (CountY- StartOperation >= IndexY)
                {
                    a = 0;
                    if(IndexX==1) a= MaxFunction(InputData, CountY - IndexY, 0,0);else 
                        a = MaxFunction(InputData, CountY - IndexY, f1, Matrix[CountX - IndexX+1, CountY - IndexY+1]);
                    Console.WriteLine($"F({IndexX}){IndexY}={a}");
                    
                    Matrix[CountX - IndexX, CountY - IndexY] = a;
                    IndexY++;
                }
                f1 = a;
                IndexX++;
            }
        }
        static int MaxFunction(int[,] InputData, int t, int F1, int FT)
        {
            int r = InputData[0, t] + FT;
            int s = InputData[1, t] - P + InputData[0, 0] + F1;

            return r > s ? r : s;
        }
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            int Duration = 7;
            int StartOperation = 1;
            int[,] InputData = {    {12,11,11,10,8,6,3 },
                                    { 11,10,8,7,5,3,1} };
            int[,] Matrix = new int[Duration - StartOperation, Duration];
            //OutputData(Matrix);
             CalcMetod(Matrix, InputData,StartOperation,Duration);
            OutputData(Matrix);

        }
    }
}
