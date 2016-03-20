#include <shark/Data/Dataset.h>

using TData = shark::RegressionDataset;


#include <shark/Algorithms/GradientDescent/Rprop.h> //resilient propagation as optimizer
#include <shark/Algorithms/Trainers/Perceptron.h>
#include <shark/Models/FFNet.h>                         //the feed forward neural network
#include <shark/ObjectiveFunctions/ErrorFunction.h>     //error function to connect data model and loss
#include <shark/ObjectiveFunctions/Loss/CrossEntropy.h> // loss during training
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>  //loss for test performance
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>
// evaluating probabilities
#include <shark/Models/ConcatenatedModel.h> //provides operator >> for concatenating models
#include <shark/Models/Softmax.h>           //transforms model output into probabilities

double TrainAndCalcAccuracy(const TData &TrainingDataSet, const TData &ValidationDataSetk) noexcept {
   shark::FFNet<shark::LogisticNeuron, shark::LinearNeuron> Network;
   Network.setStructure(2, 4, 1);
   shark::SquaredLoss<shark::RealVector, shark::RealVector> Loss;

   shark::ErrorFunction Error(TrainingDataSet, &Network, &Loss);
   shark::initRandomUniform(Network,-0.1, 0.1);

   shark::IRpropPlus optimizer;
   optimizer.init(Error);
   unsigned numberOfSteps = 1000;
   for(unsigned step = 0; step != numberOfSteps; ++step)
      optimizer.step(Error);
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

#include <shark/Data/Csv.h>
int main() {
   TData TrainingData;
   TData TestData;
   try {
      shark::importCSV(TrainingData, "/home/Void/devel/perceptron-train.csv", shark::FIRST_COLUMN, 1, ',');
   } catch(...) {
      std::cerr << "unable to read data from file " << std::endl;
      exit(1);
   }
   std::cout << "Number of data points: " << TrainingData.numberOfElements()
             << ", Input asdfghion: " << shark::inputDimension(TrainingData) << std::endl;

   TrainingData.shuffle();
   std::cout << "========================== Non normalized Perceptron ==========================" << std::endl;
   TrainAndCalcAccuracy(TrainingData, TestData);
   //   std::cout << "========================== Normalized Perceptron ==========================" << std::endl;
   //   TData NormalizedData = Normalize(Data);
   //   ChooseBestKInKNN(NormalizedData);

   return 0;
}
