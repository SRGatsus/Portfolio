using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SimplexMethod;
using SimplexMinMetod;
namespace testSimplex
{
    class Program
    {

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
        static int GetChoice(string Output, string[] Variants, string Error)
        {
            bool isNumber = false;
            string Choise = "";
            int Index = -1;
            Output += "(";
            for (int i = 0; i < Variants.Length; i++)
            {
                Output += Variants[i];
                if(i+1 != Variants.Length)
                {
                    Output += "/";
                }
            }
            Output += ")";
            while (!isNumber)
            {
                Console.WriteLine(Output);
                bool r;
                Choise = Console.ReadLine();
                for (int i = 0; i < Variants.Length; i++)
                {
                    if (Variants[i] == Choise.ToUpper())
                    {
                        Index = i;
                        break;
                    }
                }
                if (Index != -1)
                {
                    isNumber = true;
                }
                else
                {
                    Console.WriteLine(Error);
                }
            }
            return Index;
        }
        static void Main(string[] args)
        {
            bool Min = false;
            bool Debug = true;
            bool EndProgram = false;
            double[,] table = { {16,2,-1,0,-2},
                                {18,3,2,1,-3},
                                {24,-1,3,0,4},
                                {0,-2,-3,0,1}};
            double[,] table2 = { {4,0,2,1},
                                {-6,-2,-1,-2},
                                {-2,-2,1,-2},
                                {0,3,2,1}};
            int Col = 0;
            int Row = 0;

            double[,] newTable;
            while (!EndProgram)
            {
                if (Debug)
                {
                    newTable = table;
                }
                else
                {
                    string[] Variant = { "Y", "N" };
                    int Choice = GetChoice("Метод на минимум?", Variant, "Такого варианта нет");
                    GetNumber("Введите количесвто уравнение + Функций(Всех уравнение, кроме x1,x2..xn>=0)", out Col);
                    GetNumber("Введите количество различных x(x1,x2..xn)", out Row);
                    newTable = new double[Col, Row + 1];
                    Min = Choice == 0;
                    switch (Choice)
                    {
                        case 0:
                            {

                                for (int i = 0; i < newTable.GetLength(0) - 1; i++)
                                {
                                    string[] Equations = { "Y", "N" };
                                    int ChoiceEquations = GetChoice("Знак уравнение => ? ", Equations, "Такого варианта нет");
                                    int df = ChoiceEquations == 1 ? 1 : -1;
                                    for (int j = 0; j < newTable.GetLength(1); j++)
                                    {
                                        int Result;
                                        if (j == 0)
                                        {
                                            GetNumber("Введите ответ " + (i + 1) + " уравнение", out Result);

                                        }
                                        else
                                        {
                                            GetNumber("Введите значение X" + (j) + " у уравнение №" + (i + 1), out Result);
                                        }
                                        newTable[i, j] = df * Result;
                                    }

                                    Console.WriteLine();
                                }
                                newTable[newTable.GetLength(0) - 1, newTable.GetLength(1) - 1] = 0;
                                for (int j = 1; j < newTable.GetLength(1); j++)
                                {
                                    int Result;

                                    GetNumber("Введите значение X" + (j) + " у Функции ", out Result);
                                    newTable[newTable.GetLength(0) - 1, j] = Result;
                                }
                                break;
                            }
                        case 1:
                            {
                                for (int i = 0; i < newTable.GetLength(0) - 1; i++)
                                {
                                    string[] Equations = { "Y", "N" };
                                    int ChoiceEquations = GetChoice("Знак уравнение <= ? ", Variant, "Такого варианта нет");
                                    int df = ChoiceEquations == 1 ? -1 : 1;
                                    for (int j = 0; j < newTable.GetLength(1); j++)
                                    {
                                        int Result;
                                        if (j == 0)
                                        {
                                            GetNumber("Введите ответ " + (i + 1) + " уравнение", out Result);

                                        }
                                        else
                                        {
                                            GetNumber("Введите значение X" + (j) + " у уравнение №" + (i + 1), out Result);
                                        }
                                        newTable[i, j] = df * Result;
                                    }

                                    Console.WriteLine();
                                }
                                newTable[newTable.GetLength(0) - 1, newTable.GetLength(1) - 1] = 0;
                                for (int j = 1; j < newTable.GetLength(1); j++)
                                {
                                    int Result;

                                    GetNumber("Введите значение X" + (j) + " у Функции ", out Result);
                                    newTable[newTable.GetLength(0) - 1, j] = -1 * Result;
                                }
                                break;
                            }
                        default:
                            {
                                break;
                            }
                    }
                }
                for (int i = 0; i < newTable.GetLength(0); i++)
                {
                    for (int j = 0; j < newTable.GetLength(1); j++)
                        Console.Write(Math.Round(newTable[i, j], 2) + " ");
                    Console.WriteLine();
                }


                double[] result = new double[newTable.GetLength(1)];
                double[,] table_result;
                if (Min)
                {
                    SimplexMin S = new SimplexMin(newTable);
                    table_result = S.Calculate(result);
                }
                else
                {
                    Simplex S = new Simplex(newTable);
                    table_result = S.Calculate(result);
                }

                Console.WriteLine("Решенная симплекс-таблица:");
                for (int i = 0; i < table_result.GetLength(0); i++)
                {
                    for (int j = 0; j < table_result.GetLength(1); j++)
                        Console.Write(Math.Round(table_result[i, j], 2) + " ");
                    Console.WriteLine();
                }

                Console.WriteLine();
                bool ThereSolution = false;
                for (int i = 1; i < result.Length; i++)
                {
                    if (result[i] != -9999)
                    {
                        ThereSolution = true;
                    }
                }
                if (ThereSolution)
                {
                    Console.WriteLine("Решение:");
                    Console.WriteLine("F = " + result[0]);
                    for (int i = 1; i < result.Length; i++)
                    {
                        if (result[i] != -9999)
                        {
                            Console.WriteLine("X[" + i + "] = " + result[i]);
                        }
                        else
                        {
                            Console.WriteLine("X[" + i + "] = 0");
                        }
                    }
                }
                else
                {
                    Console.WriteLine("Решение нет");
                }
                string[] VariantEndProgram = { "Y", "N" };
                int ChoiceEndProgram = GetChoice("Выйти? ", VariantEndProgram, "Такого варианта нет");
                EndProgram = ChoiceEndProgram == 0;
            }
        }
    }
    }
