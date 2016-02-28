#include <shark/Data/Csv.h>
#include <shark/Data/Dataset.h>
using namespace shark;

int main() {
   ClassificationDataset Data;
   try {
      importCSV(Data, "/home/Void/devel/wine.data.txt", FIRST_COLUMN, ',');
   } catch(...) {
      std::cerr << "unable to read data from file " << std::endl;
      exit(EXIT_FAILURE);
   }

   return 0;
}
