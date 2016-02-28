import qbs

QtcProduct {
    type: ["application"]
    files: [ "main.cpp" ]

    property bool useShark: false

    property string boostHeaders: project.ide_source_tree + "/../boost/";
    property string boostLibs: project.ide_source_tree + "/../boost/stage/lib";

    cpp.includePaths: {
       var result = []
       if(useShark)
          result.push(boostHeaders)
       return result
    }
    cpp.cxxFlags: {
       var result = []
       if(useShark)
          result.push("-fopenmp=libomp", "-openmp")
       return result
    }
    cpp.libraryPaths: {
       var result = []
       if(useShark)
          result.push(project.ide_source_tree + "/../boost/stage/lib", "/usr/lib/x86_64-linux-gnu/")
       return result
    }
    cpp.dynamicLibraries: {
       var result = []
       if(useShark)
          result.push("omp", "shark", "boost_serialization")
       return result
    }
    cpp.rpaths: {
       var result = []
       if(useShark)
          result.push(project.ide_source_tree + "/../boost/stage/lib")
       return result
    }
}
