//http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/concepts/library_design/kernels.html?highlight=linearkernel
//http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/nearestNeighbor.html?highlight=knn

#include <shark/Data/CVDatasetTools.h>
#include <shark/Data/Csv.h>
#include <shark/Data/Dataset.h>

#include <shark/Algorithms/Trainers/NormalizeComponentsUnitVariance.h>
#include <shark/Models/Normalizer.h>
shark::ClassificationDataset Normalize(shark::ClassificationDataset Data) noexcept {
   shark::Normalizer<shark::RealVector> Normalizer;
   shark::NormalizeComponentsUnitVariance<shark::RealVector> NormalizingTrainer(true);
   NormalizingTrainer.train(Normalizer, Data.inputs());
   Data.makeIndependent();
   Data = shark::transformInputs(Data, Normalizer);
   return Data;
}

int main() {
   shark::UnlabeledData<shark::RealVector> Data;
   try {
      shark::importCSV(Data, "/home/Void/devel/housing.data.txt", ' ');
   } catch(...) {
      std::cerr << "unable to read data from file " << std::endl;
      exit(1);
   }
   std::cout << "Number of data points: " << Data.numberOfElements() << std::endl;

   Data.shuffle();
   //shark::ClassificationDataset NormalizedData = Normalize(Data);

   return 0;
}
