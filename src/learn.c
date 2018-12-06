#include <stdlib.h>
#include <stdio.h>

//Function to return the inverse of an augmented Matrix
double** inverse(double** matrix, int row , int col);
double** inverse(double** matrix, int row, int col){
        int i, j ,a, b, piv;
        double** ans= (double**)malloc (row*sizeof(double*));
        for(i=0;i<row;i++){
                ans[i]=(double*)malloc(col*sizeof(double));
        }

	//work from 0 to last row
        for(piv=0; piv<col; piv++){
                for(i=piv;i<col; i++){
                        double div = matrix[i][piv];
                        for(j=0;j<col*2;j++){
                                matrix[i][j]=matrix[i][j]/div;
                        }
                }

                double* firstRow= matrix[piv];
                for(a=1;a<col;a++){
                        if(a!=piv){
                                double* nextRow= matrix[a];
                                for(b=0; b<col*2; b++){
                                        nextRow[b]=nextRow[b]-firstRow[b];
                                }
                        }
                }
        }
	//work from last row up
        for(piv=col-1;piv>=0;piv--){
                for(i=piv;i>=0;i--){
                        double div= matrix[i][piv];

                        for(j=col*2-1;j>=0;j--){
                                matrix[i][j]=matrix[i][j]/div;
                        }
                }
                double* lastRow=matrix[piv];

                for(a=piv-1;a>=0;a--){
                        if(a!=piv){
                                double* nextRow=matrix[a];
                                for(b=col*2-1;b>=0;b--){
                                        nextRow[b]= nextRow[b]-lastRow[b];
                                        if(nextRow[b]== -0.0){
                                                nextRow[b]= abs(nextRow[b]);
                                        }
                                }
                        }
                }
        }

	//fill in answer matrix

        for(i=0;i<row;i++){
                for(j=0;j<col;j++){
                ans[i][j]=matrix[i][(col)+j];
                }
        }

return ans;
}


//function to return the augmented matrix
double** aug(double ** first, int row, int col);
double** aug(double ** first, int row, int col){

        double** ans=(double**) malloc(row*sizeof(double*));
        int i,j,k;
        int col2=col*2;

        for(i=0;i<row;i++){
                ans[i]=(double*)malloc(col2*sizeof(double));
        }

	for(i=0;i<row;i++){
                for(j=0;j<col;j++){
                        ans[i][j]=first[i][j];
                }

                for(k=col;k<col2;k++){
                        if(k==(col+i)){
                                ans[i][k]=1.0f;
                        }
                        else {
                              	ans[i][k]=0.0f;
                        }
                }
        }

return ans;
}

//function to find the transpose
double** transpose(double** first, int row, int col);
double** transpose(double** first, int row, int col){
        double** ans;
        int i, j;
        ans = (double**)malloc(row * sizeof(double*));

        for (i=0;i<row;i++){
                ans[i]= (double*) malloc(col * sizeof(double));
        }

        for (i=0;i<row;i++){
                for (j=0;j<col;j++){
                        ans[i][j]=first[j][i];
                }
        }
return ans;
}

//function to multiply 2 matrices
double**multiply(double** matrixA , double** matrixB, int rowA, int colA, int colB);
double** multiply(double** matrixA , double** matrixB, int rowA, int colA, int colB){
        double ** ans;
        int i,j ,k;

        ans=(double**) malloc(rowA * sizeof(double*));
        for (i=0;i<rowA;i++){
                ans[i]=(double*) malloc(colB *sizeof(double));
        }

	for(i=0;i<rowA;i++){
                for(j=0;j<colB;j++){
                        for(k=0;k<colA;k++){
                                ans[i][j]+= matrixA[i][k]*matrixB[k][j];
                        }
                }
        }

	return ans;
}

int main(int argc, char* argv[]){

        if(argc!=3){
                exit(0);
        }
	
	char* tFile=argv[1];
        char* dFile=argv[2];
        FILE * trainingFile;
        FILE * dataFile;
        trainingFile=fopen(tFile,"r");
        dataFile=fopen(dFile,"r");

        if(trainingFile==NULL || dataFile==NULL){
                printf("error\n");
                return 1;
        }
	
	int row, col, i, dataRow, checkCol,j;
        
	//skips the first row
        fscanf(trainingFile,"%*[^\n]\n");
        fscanf(trainingFile,"%d",&col);
        fscanf(trainingFile,"%d",&row);
	
	//allocate memory for rows for trainging file
        double** matX=(double**) malloc(row* sizeof(double *));
        double** matY=(double**) malloc(row* sizeof(double*));

        //allocate memory for columns
        for( i = 0; i < row; i++){
                matX[i]=(double*) malloc((col+1)* sizeof(double));
                matY[i]=(double*) malloc(sizeof(double));
               
        }

	
        //Skips the first line
        fscanf(dataFile,"%*[^\n]\n");
        fscanf(dataFile,"%d\n",&checkCol);
        fscanf(dataFile,"%d\n",&dataRow);
	
	//make sure both k values are the same
        if(checkCol!=col){
                printf("error\n");
                return 1;
        }
	

	double** matData=(double**) malloc(dataRow *sizeof(double*));

        for(i=0;i<dataRow;i++){
                matData[i]=(double*)malloc((checkCol+1)*sizeof(double));
        }

	//fill in the x array
 	for(i=0;i<row;i++){
                for(j=0;j<col+1;j++){
                        if(j==0){
                                //always 1st column is 1
                                matX[i][0]=1;
                        }
                        else if(j!=col+1){
                              	fscanf(trainingFile,"%lf\t ",&matX[i][j]);
                        }
                }
		fscanf(trainingFile, "%lf\n", &matY[i][0]);
        }
	       
	
	// fill in data array
        for(i=0;i<dataRow;i++){
		for(j=0;j<checkCol+1;j++){
                        if(j==0){
                                //1st column is always 1
				matData[i][0]=1;
                        }
                        else {
                              	fscanf(dataFile,"%lf\t ", &matData[i][j]);
                        }
                }
	
            fscanf(dataFile,"\n");
        }


	//find the transpose		
	double** transMatrix = transpose(matX, col+1, row);
	
	//find x transpose times x
	double** matW=multiply(transMatrix,matX,col+1, row, col+1);
	
	//augmented matrix of W
	double** augMat= aug(matW,col+1,col+1);
	
	//find the inverse of the augmented matrix
	double** inverseMat= inverse(augMat,col+1,col+1);
	matW=multiply(transMatrix, matY, col+1,row,1);
        matW= multiply(inverseMat, matW,col+1,col+1,1);

        double** ans= multiply(matData,matW, dataRow,col+1,1);

        for(i=0;i<dataRow;i++){
                //printf("%lf\n", ans[i][0]);
        	printf("%.0f\n",ans[i][0]);
	}

fclose(trainingFile);
fclose(dataFile);
return 0;


}
