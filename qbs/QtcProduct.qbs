import qbs 1.0

Product {
    Depends { name: "cpp" }
   //Depends { name: "Qt.core" }
   //Depends { name: "Qt.network" }
    cpp.compilerName: "clang++"; cpp.cxxStandardLibrary: "libstdc++";
    //cpp.compilerName: "g++"
   cpp.debugInformation: true
   cpp.cxxLanguageVersion: "c++14"

   Group {
      name: "The App itself"
      fileTagsFilter: "application"
      qbs.install: true
   }
}
