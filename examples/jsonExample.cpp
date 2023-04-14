/*
 * jsonExample.cpp
 *
 * How-to example for parsing and manipulating json input
 */

#include <fstream>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

using namespace rapidjson;

int main(int argc, char *argv[]) {

  std::string jsonfile = "../examples/example.json";

  // parse json input file
  std::ifstream ifs(jsonfile);
  IStreamWrapper is(ifs);
  Document d;
  d.ParseStream(is);

  // variable d contains the top json object
  std::cout << "Variable d contains json object:"
            << (d.IsObject() ? "true" : "false") << std::endl;
  std::cout << "d top level object contains " << d.MemberCount()
            << " members:" << std::endl;

  // iterating over members
  for (Value::ConstMemberIterator itr = d.MemberBegin(); itr != d.MemberEnd();
       ++itr)
    printf("%s\n", itr->name.GetString());

  // querying members
  std::cout << "d has member 'scene':"
            << (d.HasMember("scene") ? "true" : "false") << std::endl;

  // retieve member
  Value &scene = d["scene"];

  std::cout << "scene has member 'shapes':"
            << (scene.HasMember("shapes") ? "true" : "false") << std::endl;
  std::cout << "Does scene have: ?" << scene["abc"].GetFloat() << "\n";

  Value &shapes = scene["shapes"];

  // iterate through arrays
  // for (SizeType i = 0; i < shapes.Size(); i++)
  std::cout << "'shapes' is array:" << (shapes.IsArray() ? "true" : "false")
            << std::endl;
  std::cout << "'shapes' contains " << shapes.Size()
            << " elements:" << std::endl;
  for (SizeType i = 0; i < shapes.Size(); i++) { // Retrieve values through GetXXX() functions
    printf("We have object of type[%s]\n", shapes[i].GetObject()["type"].GetString());
	std::string type = shapes[i].GetObject()["type"].GetString();
	

    if (type == "sphere") {
      printf("shapes[%d] = %s radius: %f \n", i,
             shapes[i].GetObject()["type"].GetString(),
             shapes[i].GetObject()["radius"].GetFloat());

		for (auto& v: shapes[i].GetObject()["center"].GetArray()) {
			printf("%f \n", v.GetFloat());
		}



    }
  }
}
