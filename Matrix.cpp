//
// Created by Aviv9 on 13-Jun-19.
//
#include "Matrix.h"


void Matrix::checkSize(int rows, int cols) {
    if(rows==0)
        if(cols<=0){
            cerr<<"ERROR:invalid size - no values"<<endl;
            exit(1);
        }

    if(cols==0)
        if(rows<=0){
            cerr<<"ERROR:invalid size - no values"<<endl;
            exit(1);
        }
    if(rows<0||cols<0){
        cerr<<"ERROR:invalid size - negative number"<<endl;
        exit(1);
    }
}

void Matrix::destroyData(double **data) {
    for(int i=0;i<rows;i++){
        delete[] data[i];
    }
    delete[] data;
}

void Matrix::createData(int rows, int cols) {
    data = new double*[rows];
    for(int i=0;i<rows;i++) {
        data[i] = new double[cols];
    }
}

Matrix::Matrix(int rows, int cols){
    checkSize(rows, cols);
    this->rows = rows;
    this->cols = cols;
    this->createData(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            data[i][j] = 0;
        }
    }
}

Matrix::Matrix(int rows, int cols, double value) {
    checkSize(rows, cols);
    this->rows = rows;
    this->cols = cols;
    this->createData(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            data[i][j] = value;
        }
    }
}

Matrix::Matrix(int rows, int cols, double min, double max){
    checkSize(rows, cols);
    this->rows = rows;
    this->cols = cols;
    this->createData(rows,cols);
    double rand_num = fRand(min, max);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            data[i][j] = rand_num;
            rand_num = fRand(min, max);
        }
    }
}

Matrix::Matrix(const Matrix &mtx) {
    this->rows = mtx.rows;
    this->cols = mtx.cols;
    this->createData(rows, cols);
    for(int i = 0; i<rows;i++){
        for(int j=0;j<cols;j++){
            data[i][j] = mtx.data[i][j];
        }
    }
}

Matrix::Matrix(vector<vector<double>> data) {
    int rows = data.size();
    int cols = data[0].size();
    for(vector<double> row : data){
        if(row.size()!=cols){
            cerr<<"error creating a new matrix, rows have different size";
            exit(1);
        }
    }
    checkSize(rows, cols);
    this->rows = rows;
    this->cols = cols;
    this->createData(rows,cols);
    int i=0, j=0;
    for(vector<double> row : data) {
        for (double num : row) {
            this->data[i][j] = num;
            j++;
            if (j >= cols) {
                j = 0;
                i++;
            }
        }
    }
}

Matrix::~Matrix() {
    destroyData(data);
}

Matrix &Matrix::operator=(const Matrix &mtx) {
    this->destroyData(data);
    this->rows = mtx.rows;
    this->cols = mtx.cols;
    this->createData(rows, cols);
    for(int i = 0; i<rows;i++){
        for(int j=0;j<cols;j++){
            data[i][j] = mtx.data[i][j];
        }
    }
    return *this;
}

int Matrix::getCols() const {
    return cols;
}

int Matrix::getRows() const {
    return rows;
}

void Matrix::changeValue(int row, int col, double value) {
    if(row>=rows||col>=cols){
        throw InvalidIndex();
    }
    this->data[row][col] = value;
}

Matrix& Matrix::operator-() {
    for(int i = 0; i<rows ; i++){
        for(int j=0; j<cols ; j++){
            changeValue(i,j,-data[i][j]);
        }
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix &mtx) {
    int new_rows = mtx.rows;
    int new_cols = mtx.cols;
    if(new_rows!=rows||new_cols!=cols){
        cerr<<"ERROR:trying to add a "<<new_rows<<"X"<<new_cols
            <<" matrix to "<<rows<<"X"<<cols<<" matrix"<<endl;
        exit(1);
    }
    for(int i=0;i<new_rows;i++){
        for(int j=0;j<new_cols;j++){
            data[i][j] += mtx.data[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix &mtx) {
    -(*this);
    *this+=mtx;
    return -(*this);
}

Matrix &Matrix::operator*=(const Matrix &mtx) {
    if(cols != mtx.rows){
        cerr<<"ERROR:\nmultiplication does not defined: "<<rows<<"X"<<cols
        <<" matrix with "<< mtx.rows<<"X"<<mtx.cols<<" matrix"<<endl;
        exit(1);
    }
    Matrix result(rows, mtx.cols);
    for(int i=0;i<result.rows; i++){
        for(int j=0; j<result.cols; j++){
            double sum =0;
            for(int k=0;k<cols ;k++){
                sum+=(*this)(i+1,k+1)*mtx(k+1,j+1);
            }
            result.changeValue(i,j,sum);
        }
    }
    *this = result;
    return *this;
}

Matrix &Matrix::operator*=(const double& a) {
    for(int i=0;i<rows; i++){
        for(int j=0; j<cols; j++){
            double num = (*this)(i+1,j+1);
            this->changeValue(i,j,num*a);
        }
    }
    return *this;
}

double Matrix::operator()(int row, int col) const {
    if(row>this->rows||col>this->cols){
        cerr<<"error - trying to access an invalid index"<<endl;
        exit(1);
    }
    return data[row-1][col-1];
}

Matrix Matrix::T() {
    Matrix trans(cols, rows);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            trans.changeValue(j ,i, data[i][j]);
        }
    }
    return trans;
}

ostream& operator<<(ostream& os, const Matrix& mtx){
    int rows = mtx.rows;
    int cols = mtx.cols;
    os<<rows<<"X"<<cols<<"\n";
    for(int i=0;i<rows;i++){
        os<<"( ";
        for(int j=0;j<cols;j++){
            os<<mtx.data[i][j]<<" ";
        }
        os<<")\n";
    }
    return os;
}

Matrix operator+(const Matrix& mtx1 , const Matrix& mtx2){
    return Matrix(mtx1)+=mtx2;
}

Matrix operator-(const Matrix& mtx1 , const Matrix& mtx2){
    return Matrix(mtx1)-=mtx2;
}

double fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

Matrix operator*(const Matrix &mtx1, const Matrix &mtx2) {
    return Matrix(mtx1)*=mtx2;
}

Matrix operator*(const Matrix &mtx, const double& a) {
    return Matrix(mtx)*=a;
}

Matrix operator*( const double& a, const Matrix &mtx) {
    return mtx*a;
}

Matrix Matrix::addCol(const Matrix colToAdd){
    if(colToAdd.getCols() != 1){
        cerr<<"Error - trying to append non col matrix"<<endl;
        exit(1);
    }
    Matrix tmp(cols, cols+1);
    for(int i=0; i<cols; i++){
        tmp.changeValue(i,i, 1);
    }
    *this *= tmp;
    Matrix vector(1, cols);
    vector.changeValue(0, cols-1 ,1);
    tmp = colToAdd*vector;
    *this += tmp;
    return *this;
}

Matrix Matrix::addRow(const Matrix rowToAdd){
    if(rowToAdd.getRows() != 1){
        cerr<<"Error - trying to append non row matrix"<<endl;
        exit(1);
    }
    Matrix vector = rowToAdd;
    *this = (((*this).T()).addCol(vector.T())).T();
    return *this;
}
#include "Matrix.h"
