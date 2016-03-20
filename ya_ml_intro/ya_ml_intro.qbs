import qbs

Project {
   name: "YandexMLIntro"
   references: [
      "week2/1/assignment.qbs",
      "week2/2/assignment.qbs",
      "week2/3/assignment.qbs",
      "week3/1/assignment.qbs",
   ]
}

//Build boost
//./bootstrap.sh --with-toolset=clang
//./b2 -j12 toolset=clang cxxflags=-std=c++14
