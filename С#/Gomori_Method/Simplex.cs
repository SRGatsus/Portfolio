using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System;
namespace SimplexMethod
{
    public class Simplex
    {
        //source - симплекс таблица без базисных переменных
        public double[,] table; //симплекс таблица

        public int m, n;

        public List<int> basis; //список базисных переменных

        public Simplex(double[,] source)
        {
            m = source.GetLength(0);
            n = source.GetLength(1);
            table = new double[m, n + m - 1];
            basis = new List<int>();

            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < table.GetLength(1); j++)
                {
                    if (j < n)
                        table[i, j] = source[i, j];
                    else
                        table[i, j] = 0;
                    Console.Write(table[i, j] + " ");
                }
                //выставляем коэффициент 1 перед базисной переменной в строке
                if ((n + i) < table.GetLength(1))
                {
                    table[i, n + i] = 1;
                    basis.Add(n + i);
                    Console.Write(table[i, n + i] + " ");
                }
                Console.WriteLine();
            }

            n = table.GetLength(1);
            Console.WriteLine();
        }

        //result - в этот массив будут записаны полученные значения X
        public double[,] Calculate(double[] result)
        {
            int mainCol, mainRow; //ведущие столбец и строка

            while (!IsItEnd())
            {
                mainCol = findMainCol();
                mainRow = findMainRow(mainCol);
                basis[mainRow] = mainCol;

                double[,] new_table = new double[m, n];

                for (int j = 0; j < n; j++)
                    new_table[mainRow, j] = table[mainRow, j] / table[mainRow, mainCol];

                for (int i = 0; i < m; i++)
                {
                    if (i == mainRow)
                        continue;

                    for (int j = 0; j < n; j++)
                        new_table[i, j] = table[i, j] - table[i, mainCol] * new_table[mainRow, j];
                }
                table = new_table;
                for (int i = 0; i < table.GetLength(0); i++)
                {
                    for (int j = 0; j < table.GetLength(1); j++)
                        Console.Write(Math.Round(table[i, j],2) + " ");
                    Console.WriteLine();
                }
                Console.WriteLine();

            }

            result[0] = table[table.GetLength(0) - 1, 0];
            //заносим в result найденные значения X

                for (int i = 1; i < result.Length; i++)
                {
                    int k = basis.IndexOf(i);
                    if (k != -1)
                        result[i] = table[k, 0];
                    else
                        result[i] =-999999;
                }
            return table;
        }

        private bool IsItEnd()
        {
            bool flag = true;

            for (int j = 1; j < n; j++)
            {
                if (table[m - 1, j] < 0)
                {
                    flag = false;
                    break;
                }
            }

            return flag;
        }
        public void Gomor(double[] result)
        {
            int mainCol=0;
            int  mainRow= table.GetLength(0)-2;
            double min = 1000;            
            for (int i = 1; i < table.GetLength(1)-1; i++)
            {
                if ((table[table.GetLength(0) - 2, i]!=0))
                {
                    if (min > table[table.GetLength(0) - 1, i] / table[table.GetLength(0) - 2, i])
                    {
                        min = table[table.GetLength(0) - 1, i] / table[table.GetLength(0) - 2, i];
                        mainCol = i;
                    }
                }
            }
            double[,] new_table = new double[table.GetLength(0), table.GetLength(1)];
            for (int i = 0; i < table.GetLength(0); i++)
            {
                for (int j = 0; j < table.GetLength(1); j++)
                    new_table[i,j] = table[i, j];
               
            }
            basis[mainRow] = mainCol;
            Console.WriteLine("Столбец:  " + mainCol);
            Console.WriteLine("Строка:  " + mainRow);
            for (int j = 0; j < n; j++)
                new_table[mainRow, j] = table[mainRow, j] / table[mainRow, mainCol];

            for (int i = 0; i < table.GetLength(0)-1; i++)
            {
                if (i == mainRow)
                    continue;

                for (int j =0; j < table.GetLength(1); j++)
                    new_table[i, j] = table[i, j] -table[i, mainCol] * new_table[mainRow, j];
            }
            table = new_table;
            for (int i = 0; i < table.GetLength(0); i++)
            {
                for (int j = 0; j < table.GetLength(1); j++)
                    Console.Write(Math.Round(table[i, j], 2) + " ");
                Console.WriteLine();
            }
            Console.WriteLine();
            result[0] = table[table.GetLength(0) - 1, 0];
            //заносим в result найденные значения X

            for (int i = 1; i < result.Length; i++)
            {
                int k = basis.IndexOf(i);
                if (k != -1)
                    result[i] = Math.Round(table[k, 0], 2);
                else
                    result[i] = -999999;
            }
        }
        private int findMainCol()
        {
            int mainCol = 1;

            for (int j = 2; j < n; j++)
                if (table[m - 1, j] < table[m - 1, mainCol])
                    mainCol = j;
            Console.WriteLine("Столбец:  " + mainCol);
            return mainCol;
        }

        private int findMainRow(int mainCol)
        {
            int mainRow = 0;

            for (int i = 0; i < m - 1; i++)
                if (table[i, mainCol] > 0)
                {
                    mainRow = i;
                    break;
                }

            for (int i = mainRow + 1; i < m - 1; i++)
                if ((table[i, mainCol] > 0) && ((table[i, 0] / table[i, mainCol]) < (table[mainRow, 0] / table[mainRow, mainCol])))
                    mainRow = i;
            Console.WriteLine("ряд:  " + mainRow);
            return mainRow;
        }


    }
}
