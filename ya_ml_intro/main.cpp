#include <shark/Data/CVDatasetTools.h>
#include <shark/Data/Csv.h>
#include <shark/Data/Dataset.h>



#include <shark/Algorithms/NearestNeighbors/TreeNearestNeighbors.h>
#include <shark/Models/NearestNeighborClassifier.h>
#include <shark/Models/Trees/KDTree.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
double TrainAndCalcAccuracy(const shark::ClassificationDataset &TrainingDataSet,
                            const shark::ClassificationDataset &ValidationDataSet,
                            size_t                              k) noexcept {
   shark::KDTree<shark::RealVector> Tree(TrainingDataSet.inputs());
   shark::TreeNearestNeighbors<shark::RealVector, unsigned int> Algo(TrainingDataSet, &Tree);
   shark::NearestNeighborClassifier<shark::RealVector> KNN(&Algo, k);

   shark::ZeroOneLoss<unsigned int> Loss;
   shark::Data<unsigned int>        Prediction;

//   Prediction                           = KNN(TrainingDataSet.inputs());
//   const double TrainingDataSetAccuracy = 1. - Loss.eval(TrainingDataSet.labels(), Prediction);
//   std::cout << k << "-KNN on training set accuracy: " << TrainingDataSetAccuracy << std::endl;

   Prediction                             = KNN(ValidationDataSet.inputs());
   const double ValidationDataSetAccuracy = 1. - Loss.eval(ValidationDataSet.labels(), Prediction);
   //std::cout << k << "-KNN on validation set accuracy: " << ValidationDataSetAccuracy << std::endl;

   return ValidationDataSetAccuracy;
}

void ChooseBestKInKNN(shark::ClassificationDataset &Data) noexcept {
   static const size_t MinK = 1;
   static const size_t MaxK = 50;

   shark::CVFolds<shark::ClassificationDataset> Folds = shark::createCVSameSize(Data, 5);

   double BestAccuracy = 0;
   size_t BestK        = 0;

   for(size_t k = MinK; k <= MaxK; ++k) {
      double CrossValidatedAccuracyForThisK = 0.;
      for(size_t FoldN = 0; FoldN < Folds.size(); ++FoldN) {
         const shark::ClassificationDataset &TrainingDataSet   = Folds.training(FoldN);
         const shark::ClassificationDataset &ValidationDataSet = Folds.validation(FoldN);
         CrossValidatedAccuracyForThisK += TrainAndCalcAccuracy(TrainingDataSet, ValidationDataSet, k);
      }
      CrossValidatedAccuracyForThisK /= Folds.size();
      std::cout << k << " " << CrossValidatedAccuracyForThisK << std::endl;

      if(CrossValidatedAccuracyForThisK > BestAccuracy) {
         BestAccuracy = CrossValidatedAccuracyForThisK;
         BestK        = k;
      }
   }
   std::cout << "Best: " << BestK << "-kNN, its Accuracy: " << BestAccuracy << std::endl;
}

#include <shark/Algorithms/Trainers/NormalizeComponentsUnitVariance.h>
#include <shark/Models/Normalizer.h>
shark::ClassificationDataset Normalize(shark::ClassificationDataset Data) noexcept {
   shark::Normalizer<shark::RealVector> Normalizer;
   shark::NormalizeComponentsUnitVariance<shark::RealVector> NormalizingTrainer(true);
   NormalizingTrainer.train(Normalizer, Data.inputs());
   Data = shark::transformInputs(Data, Normalizer);
   return Data;
}

int main() {
   shark::ClassificationDataset Data;
   try {
      shark::importCSV(Data, "/home/Void/devel/wine.data.txt", shark::FIRST_COLUMN, ',');
   } catch(...) {
      std::cerr << "unable to read data from file " << std::endl;
      exit(1);
   }
   std::cout << "Number of data points: " << Data.numberOfElements()
             << ", number of classes: " << shark::numberOfClasses(Data)
             << ", Input dimension: " << shark::inputDimension(Data) << std::endl;

   Data.shuffle();
   std::cout << "========================== Non normalized kNN ==========================" << std::endl;
   ChooseBestKInKNN(Data);
   std::cout << "========================== Normalized kNN ==========================" << std::endl;
   shark::ClassificationDataset NormalizedData = Normalize(Data);
   ChooseBestKInKNN(NormalizedData);

   return 0;
}
