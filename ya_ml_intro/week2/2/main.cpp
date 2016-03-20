// http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/concepts/library_design/kernels.html?highlight=linearkernel
// http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/nearestNeighbor.html?highlight=knn

#include <shark/Data/CVDatasetTools.h>
#include <shark/Data/Csv.h>
#include <shark/Data/Dataset.h>

using TData = shark::RegressionDataset;


#include <cmath>
#include <shark/Models/Kernels/AbstractKernelFunction.h>
class TMinkowskiKernel : public shark::AbstractKernelFunction<shark::RealVector> {
public:
   using TInputType           = shark::RealVector;
   using TBase                = shark::AbstractKernelFunction<TInputType>;
   using TConstInputReference = typename TBase::ConstInputReference;
   TMinkowskiKernel(double p) {
      m_p = p;
      //m_features |= TBase::HAS_FIRST_PARAMETER_DERIVATIVE;
      //m_features |= TBase::HAS_FIRST_INPUT_DERIVATIVE;
      m_features |= TBase::IS_NORMALIZED;
      m_features |= TBase::SUPPORTS_VARIABLE_INPUT_SIZE;
   }

   virtual void eval(ConstBatchInputReference batchX1,
                     ConstBatchInputReference batchX2,
                     shark::RealMatrix &      result,
                     shark::State &           state) const override {
      shark::RealVector f = shark::get(batchX1, 1);
   }
   double eval(TConstInputReference x1, TConstInputReference x2) const override {
      SIZE_CHECK(x1.size() == x2.size());
      double Result = 0;
      for(size_t i = 0; i < x1.size(); ++i) {
         Result += std::pow(std::abs(x1[i] - x2[i]), m_p);
      }
      Result = 1. - std::pow(Result, 2. / m_p) / 2.;
      return Result;
   }

private:
   double m_p;
};

#include <shark/Algorithms/NearestNeighbors/SimpleNearestNeighbors.h>
#include <shark/Models/NearestNeighborRegression.h>
#include <shark/Models/Trees/KDTree.h>
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>
double TrainAndCalcAccuracy(const TData &TrainingDataSet, const TData &ValidationDataSet, double p) noexcept {
   TMinkowskiKernel                 Kernel(p);
   //shark::MonomialKernel Kernel(p);
   shark::SimpleNearestNeighbors<shark::RealVector, shark::RealVector> Algo(TrainingDataSet, &Kernel);


   const size_t                                        k = 5;
   shark::NearestNeighborRegression<shark::RealVector> KNN(&Algo, k);
   KNN.setDistanceWeightType(shark::NearestNeighborRegression<shark::RealVector>::ONE_OVER_DISTANCE);

   shark::SquaredLoss<shark::RealVector> Loss;
   shark::Data<shark::RealVector>        Prediction;

   //   Prediction                           = KNN(TrainingDataSet.inputs());
   //   const double TrainingDataSetAccuracy = 1. - Loss.eval(TrainingDataSet.labels(), Prediction);
   //   std::cout << k << "-KNN on training set accuracy: " << TrainingDataSetAccuracy << std::endl;

   Prediction                             = KNN(ValidationDataSet.inputs());
   const double ValidationDataSetAccuracy = 1. - Loss.eval(ValidationDataSet.labels(), Prediction);
   // std::cout << k << "-KNN on validation set accuracy: " << ValidationDataSetAccuracy << std::endl;

   return ValidationDataSetAccuracy;
}


void ChooseBestPInMinkowskiKernel(TData &Data) noexcept {
   static const size_t Steps = 200;
   static const double MinP  = 1.;
   static const double MaxP  = 10.;

   shark::CVFolds<TData> Folds = shark::createCVSameSize(Data, 5);

   double BestAccuracy = 0;
   double BestP        = 0;

   for(size_t k = 0; k <= Steps; ++k) {
      double p = MinP + k * (MaxP - MinP) / Steps;

      double CrossValidatedAccuracyForThisK = 0.;
      for(size_t FoldN = 0; FoldN < Folds.size(); ++FoldN) {
         const TData &TrainingDataSet   = Folds.training(FoldN);
         const TData &ValidationDataSet = Folds.validation(FoldN);
         CrossValidatedAccuracyForThisK += TrainAndCalcAccuracy(TrainingDataSet, ValidationDataSet, p);
      }
      CrossValidatedAccuracyForThisK /= Folds.size();
      std::cout << k << " " << CrossValidatedAccuracyForThisK << std::endl;

      if(CrossValidatedAccuracyForThisK > BestAccuracy) {
         BestAccuracy = CrossValidatedAccuracyForThisK;
         BestP        = k;
      }
   }
   std::cout << "Best: " << BestP << "-kNN, its Accuracy: " << BestAccuracy << std::endl;
}



#include <shark/Algorithms/Trainers/NormalizeComponentsUnitVariance.h>
#include <shark/Models/Normalizer.h>
TData Normalize(TData Data) noexcept {
   shark::Normalizer<shark::RealVector>                      Normalizer;
   shark::NormalizeComponentsUnitVariance<shark::RealVector> NormalizingTrainer(true);
   NormalizingTrainer.train(Normalizer, Data.inputs());
   Data.makeIndependent();
   Data = shark::transformInputs(Data, Normalizer);
   return Data;
}

int main() {
   TData Data;
   try {
      shark::importCSV(Data, "/home/Void/devel/boston.txt", shark::LAST_COLUMN, 1, ' ');
   } catch(...) {
      std::cerr << "unable to read data from file " << std::endl;
      exit(1);
   }
   std::cout << "Number of data points: " << Data.numberOfElements() << ", Input dimension: " << shark::inputDimension(Data)
             << std::endl;

   Data.shuffle();
   TData NormalizedData = Normalize(Data);
   ChooseBestPInMinkowskiKernel(Data);

   return 0;
}
