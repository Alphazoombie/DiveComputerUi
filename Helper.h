namespace Helper
{
    void concatCharArrays(char* res, char* arr1, char* arr2)
    {
        int i = 0, j = 0;
        for (i;arr1[i]!='\0';i++){
            res[i] = arr1[i];
        }
        for (i,j;arr2[j]!='\0';i++,j++){
            res[i] = arr2[j];
        }
    }
}
