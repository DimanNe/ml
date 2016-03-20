// http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/nearestNeighbor.html?highlight=knn

#include <shark/Data/CVDatasetTools.h>
#include <shark/Data/Csv.h>
#include <shark/Data/Dataset.h>

#include <shark/Algorithms/Trainers/CSvmTrainer.h>  // the C-SVM trainer
#include <shark/Data/DataDistribution.h>            //includes small toy distributions
#include <shark/Models/Kernels/GaussianRbfKernel.h> //the used kernel for the SVM
#include <shark/Models/Kernels/LinearKernel.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h> //used for evaluation of the classifier

int main() {
   shark::ClassificationDataset Data;
   try {
      shark::importCSV(Data, "/home/Void/devel/svm-data.csv", shark::FIRST_COLUMN, ',');
   } catch(...) {
      std::cerr << "unable to read data from file " << std::endl;
      exit(1);
   }
   std::cout << "Number of data points: " << Data.numberOfElements()
             << ", number of classes: " << shark::numberOfClasses(Data)
             << ", Input dimension: " << shark::inputDimension(Data) << std::endl;

   Data.shuffle();

   shark::LinearKernel<>                      Kernel;
   shark::KernelClassifier<shark::RealVector> KernelClassifier; // (affine) linear function in kernel-induced feature space

   const double C    = 1000.0; // regularization parameter
   const bool   Bias = true;   // use bias/offset parameter

   shark::CSvmTrainer<shark::RealVector> Trainer(&Kernel, C, Bias);
   Trainer.train(KernelClassifier, Data);

   std:: cout << KernelClassifier.decisionFunction().basis() << std::endl;

   return 0;
}
