import qbs 1.0

Application {
   name: "ya_ml_intro"
   files: [
      "main.cpp"           ,
   ]
   Depends { name: "cpp" }
   //Depends { name: "Qt.core" }
   //Depends { name: "Qt.network" }
   cpp.compilerName: "clang++"
   cpp.cxxStandardLibrary: "libstdc++"
   cpp.debugInformation: true
   cpp.cxxLanguageVersion: "c++14"

   cpp.dynamicLibraries: [ "shark", "boost_serialization" ]
   cpp.libraryPaths: ["/usr/lib/x86_64-linux-gnu/"]

   cpp.cxxFlags: ["-fopenmp=libomp"]

   Group {
      name: "The App itself"
      fileTagsFilter: "application"
      qbs.install: true
   }
}
