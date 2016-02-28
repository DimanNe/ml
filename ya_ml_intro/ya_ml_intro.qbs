import qbs 1.0

Application {
   name: "ya_ml_intro"
   files: [
      "main.cpp"
   ]
   Depends { name: "cpp" }
   //Depends { name: "Qt.core" }
   //Depends { name: "Qt.network" }

   cpp.compilerName: "clang++"; cpp.cxxStandardLibrary: "libstdc++";
   //cpp.compilerName: "g++"

   cpp.debugInformation: true
   cpp.cxxLanguageVersion: "c++14"
   cpp.cxxFlags: ["-fopenmp=libomp", "-openmp"]
   cpp.includePaths: [ "/home/Void/devel/boost" ]

   cpp.libraryPaths: [
      "/home/Void/devel/boost/stage/lib",
      "/usr/lib/x86_64-linux-gnu/",
   ]
   cpp.dynamicLibraries: [
      "omp",
      "shark",
      "boost_serialization" ]

   Group {
      name: "The App itself"
      fileTagsFilter: "application"
      qbs.install: true
   }
}


//Build boost
//./bootstrap.sh --with-toolset=clang
//./b2 -j12 toolset=clang cxxflags=-std=c++14
