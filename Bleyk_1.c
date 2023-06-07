//Алгоритм блейка начало (обобщённое склеивание)
//Уткиров Рустам М1 - 22 дата создания 02.06.2023
//если что то не понятно пишите в тг @Novichok271


//УБЕДИТЕЛЬНАЯ ПРОСЬБА НЕ КОПИРУЙТЕ КОД ЧТОБЫ СДАТЬ ЗАДАЧУ 
//ПОПЫТАЙТЕСЬ ПОНЯТЬ, ПОМНИТЕ ЕСЛИ ВЫ ТУПО СКОПИРУЕТЕ 
//ВАМ ВСЁ РАВНО НУЖНО ЕГО ЗАЩИТИТЬ И ВЫ ПРОСТО ПОДСТАВИТЕ ДРУЗЕЙ


#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#pragma warning (disable : 4996)
#define min(a, b) a < b ? a : b

//Функция для чтения с файла. Возвращает двумерный массив мономов
char** readFromFile(const char* fileName, int* n, int* monomsCount)
{
    char** arrayOfMonoms = NULL;
    FILE* in = fopen(fileName, "r");
    if (fscanf(in, "%d %d\n", n, monomsCount) == 2 && (*n > 0 && *monomsCount > 0))
    {
        int _monomsCount = *monomsCount;
        int _n = *n;
        int size = pow(2, _n);
        arrayOfMonoms = (char**)calloc(size ,sizeof(char*));
        for (int i = 0; i != size; ++i)
        {
            arrayOfMonoms[i] = (char*)calloc(_n ,sizeof(char));
        }

        char sym;
        for (int i = 0; i != _monomsCount; ++i)
        {
            for (int j = 0; j != _n + 2; ++j)
            {
                if (fscanf(in, "%c", &sym) == 1 && sym != '\n' && sym != '\r')
                {
                    if (sym == '*')
                    {
                        arrayOfMonoms[i][j] = '2';
                    }
                    else
                    {
                        arrayOfMonoms[i][j] = sym;
                    }
                }
                else
                {
                    continue;
                }
            }
        }
    }
    fclose(in);
    return arrayOfMonoms;
}


//Функция проверяет склеиваемы ли наборы и если да, то возвращае индекс,
// где можно их склеить, в противном случае возвращает -1
int getIndexOfThePlaceOfGluing(char* firstMonom, char* secondMonom, const int n)
{
    int count = 0;
    int index = 0;
    for (int i = 0; i != n; ++i)
    {
        if (firstMonom[i] != secondMonom[i] && (firstMonom[i] != '2' && secondMonom[i] != '2'))
        {
            index = i;
            ++count;
        }
    }
    if (count == 1)
    {
        return index;
    }
    return -1;
}


//Возвращает указатель на начало массива, который интерпретируется как обобщенное склеивание пары жлементов
char* getGluedPairs(char* firstMonom, char* secondMonom, const int n, const int index)
{
    char* gluedPairs = (char*)malloc(sizeof(char) * n);
    for (int i = 0; i != n; ++i)
    {
        if (i == index)
        {
            gluedPairs[i] = '2';
        }
        else
        {
            gluedPairs[i] = min(firstMonom[i], secondMonom[i]);
        }
    }
    return gluedPairs;
}


//Проверяет есть ли моном в полиноме, если да то возвращает 1 и 0 в проитивном случае
int isAdded(char** arrayOfMonoms, char* chekedMonom, const int n, const int monomsCount)
{
    int counter;
    for (int i = 0; i != monomsCount; ++i)
    {
        counter = 0;
        for (int j = 0; j != n; ++j)
        {
            if (arrayOfMonoms[i][j] == chekedMonom[j])
            {
                ++counter;
            }
        }
        if (counter == n)
        {
            return 1;
        }
    }
    return 0;
}


//Добавляет моном в обобщенное склеивание
void addMonomInGeneralizedGluing(char** arrayOfMonoms, char* monom, int* monomsCount, int n)
{
    for (int i = 0; i != n; ++i)
    {
        arrayOfMonoms[*monomsCount][i] = monom[i];
    }
    ++(*monomsCount);
}


//Перевод из троичной системы счисления в десятичную
int convert(char* arr, int n)
{
    int sum = 0;
    for (int i = 0; i != n; ++i)
    {
        sum += (arr[i] - '0') * pow(3, n - i - 1);
    }
    return sum;
}

//Сортировка на основании сортировки пузырьком
void sort(char** arrayOfMonoms, const int n, const int monomsCount)
{
    char* tmp = (char*)malloc(sizeof(char) * n);
    for (int i = 0; i != monomsCount; ++i)
    {
        for (int j = 0; j != monomsCount - 1; ++j)
        {
            if (convert(arrayOfMonoms[j], n) > convert(arrayOfMonoms[j + 1], n))
            {
                for (int k = 0; k != n; ++k)
                {
                    tmp[k] = arrayOfMonoms[j][k];
                    arrayOfMonoms[j][k] = arrayOfMonoms[j + 1][k];
                    arrayOfMonoms[j + 1][k] = tmp[k];
                }
            }
        }
    }
    free(tmp);
}

// Получить обобщенное склеивание
void getGeneralizedGluing(char** arrayOfMonoms, const int n, int* monomsCount)
{
    char* arr = NULL;
    for (int i = 0; i != *monomsCount; ++i)
    {
        for (int j = i + 1; j != *monomsCount; ++j)
        {
            int index = getIndexOfThePlaceOfGluing(arrayOfMonoms[i], arrayOfMonoms[j], n);
            if (index != -1)
            {
                arr = getGluedPairs(arrayOfMonoms[i], arrayOfMonoms[j], n, index);
                if (!isAdded(arrayOfMonoms, arr, n, *monomsCount))
                {
                    addMonomInGeneralizedGluing(arrayOfMonoms, arr, monomsCount, n);
                }
                free(arr);
                arr = NULL;
            }
        }
    }
    sort(arrayOfMonoms, n, *monomsCount);
}

void writeToFile(const char* fileName, char** arrayOfMonoms, int n, int monomsNumber)
{
    FILE* out = fopen(fileName, "w");
    fprintf(out, "%d %d\n", n, monomsNumber);
    for (int i = 0; i != monomsNumber; ++i)
    {
        for (int j = 0; j != n; ++j)
        {
            if (arrayOfMonoms[i][j] == '2')
            {
                fprintf(out, "%c", '*');
            }
            else
            {
                fprintf(out, "%c", arrayOfMonoms[i][j]);
            }
        }
        fprintf(out, "%c", '\n');
    }
    fclose(out);
}

int main(int argc, char* argv[])
{
    int n;
    int monomsNumber;
    char** arrayOfMonoms = readFromFile("input.txt", &n, &monomsNumber);
    getGeneralizedGluing(arrayOfMonoms, n, &monomsNumber);
    writeToFile("output.txt", arrayOfMonoms, n, monomsNumber);
    
    int size = pow(2, n);

    for (int i = 0; i != size; ++i)
    {
        free(arrayOfMonoms[i]);
    }
    free(arrayOfMonoms);
    return 0;
}