#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include "hdf5.h"
#include <chrono>
#define DATASET_NAME "Happiness"

int main(int argc,char* argv[]){
    if(!(argc==4))
        std::cerr << "input error"<<std::endl;

    int N1,N2,iteration;
    N1 = std::atoi(argv[1]);
    N2 = std::atoi(argv[2]);
    iteration = std::atoi(argv[3]);


    std::srand(time(NULL));
    std::vector<int> v1(N1*N2);

    for(int i=0;i<N1*N2; i++){
        v1[i]=std::rand();
    }

    //normal
    auto start1 = std::chrono::high_resolution_clock::now();

    std::ofstream writefile("normal"+std::to_string(iteration)+".txt");
    if(!writefile) {
        std::cerr<< "Normal file error"<<std::endl;
        return -1;
    }
    for(int i=0;i<N1*N2;i++) {
        writefile << v1[i] <<" ";
    }

    auto end1=std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed1 = end1 - start1;
    std::cout <<"normal time :" <<elapsed1.count()<<std::endl;

    writefile.close();


    //hdf5
    hid_t  file, datatype, dataspace, dataset;
    hsize_t dim[2]; dim[0]=N1; dim[1]=N2;
    herr_t  status;

    auto start2 = std::chrono::high_resolution_clock::now();

    file= H5Fcreate(("hdf5"+std::to_string(iteration)+".h5").c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    datatype = H5Tcopy(H5T_NATIVE_INT);
    status = H5Tset_order(datatype, H5T_ORDER_LE);
    dataspace = H5Screate_simple(2,dim,NULL);

    dataset = H5Dcreate2(file, DATASET_NAME, datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, v1.data());

    auto end2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed2 = end2 - start2;
    std::cout <<"hdf5 time :" << elapsed2.count() <<std::endl;

    status = H5Sclose(dataspace);
    status = H5Tclose(datatype);
    status = H5Dclose(dataset);
    status = H5Fclose(file);

    for(int i=0;i<10;i++){
        std::cout<<v1[i]<<" ";
    }
    std::cout<<std::endl;
    return 0;
}