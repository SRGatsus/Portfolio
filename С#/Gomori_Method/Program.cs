using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SimplexMethod;
namespace testSimplex
{
    class Program
    {
        class ElemntZLP
        {
            public bool ThereSolution;
            public bool Decision;
            public List<int> x=new List<int>();
            public int Count=0;
            public int Limitation;
            public double[] result;
        }
        static List<ElemntZLP> masZLP = new List<ElemntZLP>();
        static void  GetNumber(string value, out int Number)
        {
            bool isNumber = false;
            string stringNumber = "";
            int Number2=0;

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
        static void GetNumber(string value, out double Number)
        {
            bool isNumber = false;
            string stringNumber = "";
            double Number2 = 0;
            while (!isNumber)
            {
                Console.WriteLine(value);
                bool r;
                stringNumber = Console.ReadLine();

                r = double.TryParse(stringNumber, out Number2);

                if (r)
                {
                    isNumber = true;
                }
                else
                {
                    Console.WriteLine("Error:Вы ввели не число");
                }
            }
            Number = Number2;
        }
         static void Main(string[] args)
        {

            double[,] table = { {14, 1,  4},
                                {12, 2,  3},
                                {0,  -2,  -3}
                                };
            
          
           int Col = 3;
            int Row = 2;
            //GetNumber("Введите количесвто уравнение + Функций(Всех уравнение, кроме x1,x2..xn>=0)", out Col);
            //GetNumber("Введите количество различных x(x1,x2..xn)", out Row);
            double[,] newTable = table;

            /* for (int i = 0; i < newTable.GetLength(0)-1; i++)
             {
                 for (int j = 0; j < newTable.GetLength(1); j++)
                 {
                     int Result;
                     if (j == 0) 
                     {
                         GetNumber("Введите ответ "+(i+1)+" уравнение", out Result);

                     } 
                     else
                     {
                         GetNumber("Введите значение X" + (j) + " у уравнение №" + (i + 1), out Result);
                     }
                     newTable[i, j] = Result;
                 }

                 Console.WriteLine();
             }
             newTable[newTable.GetLength(0) - 1, newTable.GetLength(1)-1] = 0;
             newTable = table;
             for (int j = 1; j < newTable.GetLength(1); j++)
             {
                 int Result;

                GetNumber("Введите значение X" + (j) + " у Функции ", out Result);
                 newTable[newTable.GetLength(0) - 1, j] = Result;
             }*/
            for (int i = 0; i < newTable.GetLength(0); i++)
            {
                for (int j = 0; j < newTable.GetLength(1); j++)
                    Console.Write(Math.Round(newTable[i, j], 2) + " ");
                Console.WriteLine();
            }


            double[] result = new double[Row + 1];
            double[,] table_result;
            Simplex S = new Simplex(newTable);
            table_result = S.Calculate(result);
            int countElemnt = 0;
            masZLP.Add(new ElemntZLP());
            masZLP[countElemnt].ThereSolution = !CheckResult(result);
            Console.WriteLine("F = " + result[0]);
            for (int i = 1; i < Row + 1; i++)
            {
                Console.WriteLine("X[" + i + "] = " + result[i]);
            }
            Console.WriteLine();
            if (IsInt(result) == -1)
            {
                Console.WriteLine("Решение:");
                Console.WriteLine("F = " + result[0]);
                for (int i = 1; i < Row + 1; i++)
                {
                    Console.WriteLine("X[" + i + "] = " + result[i]);
                }
                Console.ReadLine();
            }
            else
            {
                while (IsInt(result) != -1)
                {
                    Console.WriteLine("x["+ IsInt(result) + "]=" + result[IsInt(result)] + " не целое");
                    Console.WriteLine();
                    newTable = S.table;
                    S.table = new double[S.table.GetLength(0)+1, S.table.GetLength(1) + 1];
                    for (int i = 0; i < newTable.GetLength(0)-1; i++)
                    {
                        for (int j = 0; j < newTable.GetLength(1); j++)
                        {
                            S.table[i, j] = newTable[i,j];
                        }
                    }
                    int k = S.basis.IndexOf(IsInt(result))+1;
                    for (int i = 0; i < S.table.GetLength(1)-1; i++)
                    {
                        double fhjk = newTable[k, i];
                        if (fhjk < 0)
                        {
                            double fd = (Math.Ceiling(fhjk) - 1)*-1;
                            fhjk = fd + fhjk;

                        }
                        else
                        {
                            fhjk = fhjk % 1;
                        }
                        S.table[S.table.GetLength(0) - 2, i] = -1*fhjk;
                        S.table[S.table.GetLength(0)-1,i] = -1*newTable[newTable.GetLength(0) - 1, i];
                    }
                    for (int i = 0; i < S.table.GetLength(0); i++)
                    {
                        S.table[i, S.table.GetLength(1) - 1] = 0;
                    }

                    S.table[S.table.GetLength(0) - 2, S.table.GetLength(1) - 1] = 1;
                    S.basis.Add(S.table.GetLength(1) + 1);
                    for (int i = 0; i < S.table.GetLength(0); i++)
                    {
                        for (int j = 0; j < S.table.GetLength(1); j++)
                            Console.Write(Math.Round(S.table[i, j], 2) + " ");
                        Console.WriteLine();
                    }
                    S.n = S.table.GetLength(1);
                    Console.WriteLine();
                    S.Gomor(result);
                }
                Console.WriteLine("Решение:");
                Console.WriteLine("F = " + -result[0]);
                for (int i = 1; i < Row + 1; i++)
                {
                    Console.WriteLine("X[" + i + "] = " + result[i]);
                }
                Console.ReadLine();
            }
          
        }
        
       static  bool CheckResult(double[] val)
        {
            bool Check= false;
            for (int i = 1; i < val.Length; i++)
            {
                if(val[i]!= -999999)
                {
                    Check = true;
                }
                else
                {
                    val[i] = 0;
                }
            }
            return Check;
        }
        static  void SaveResult(double[] val,int index)
        {
            masZLP[index].result = new double[val.Length];
            for (int i = 1; i < val.Length; i++)
            {
                masZLP[index].result[i] = val[0];
            }
        }
        double MaxElementX(double[] val)
        {
            double Check = val[0];
            for (int i = 1; i < val.Length; i++)
            {
                if (val[i] > Check)
                {
                    Check = val[i];
                }
            }
            return Check;
        }
        static  int IsInt(double[] val)
        {
            int Check = -1;
            if (val[1] % 1 != 0) Check = 1;
            else Check = -1;
            double val2 = val[0];
            for (int i = 2; i < val.Length; i++)
            {
                if (val[i]%1!=0&& (val2 % 1) <=(val[i] % 1))
                {
                    Check = i;
                }
            }
            return Check;
        }
        static int isEnd()
        {
            int end =-1;
            for (int i = 0; i < masZLP.Count; i++)
            {
                if (!masZLP[i].Decision)
                {
                    end = i;
                    break;
                }
            }
            return end;
        }
    }
}
