#include <iostream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <string>
#include <utility>
#include <queue>
#include <fstream>
#include <istream>
#include <memory>

using CitiesSet_t         = std::unordered_set<std::string>;

struct Node_t {
  bool        visited;
  std::shared_ptr<CitiesSet_t> connectedCities;
  Node_t() : visited(false) , connectedCities(std::make_shared<CitiesSet_t>()) {};
};

using CitiesMap_t         = std::unordered_map<std::string, Node_t>;
using CitiesMapItr_t      = CitiesMap_t::iterator;
using NodeQueue_t         = std::queue<Node_t>;

void insertCity(CitiesMap_t&       city_map, 
		const std::string& city1, 
		const std::string& city2){

  auto c_itr = city_map.find(city1);
  if(c_itr == city_map.end() ) {
    Node_t      node;
    std::pair<CitiesMapItr_t, bool> rv = 
      city_map.insert(std::pair<std::string, Node_t>(city1, node));
    c_itr = rv.first;
  }
  if(c_itr != city_map.end() ) {
    c_itr->second.connectedCities.get()->insert(city2);
  }
  return;
}

bool checkCitiesExist(const CitiesMap_t& city_map,
		      const std::string& city1, 
		      const std::string& city2) {

  auto c_itr1 = city_map.find(city1);
  auto c_itr2 = city_map.find(city2);
  auto c_end  = city_map.end();
  bool rv=false;
  if((c_itr1 != c_end) && 
     (c_itr2 != c_end) &&
     (!c_itr1->second.connectedCities.get()->empty()) &&
     (!c_itr2->second.connectedCities.get()->empty())) {
      rv = true;
  }
  return rv;
}

bool findCity(CitiesMap_t&       city_map, 
	      const std::string& begin_city,
	      const std::string& city) {

  NodeQueue_t node_queue;
  auto c_itr = city_map.find(begin_city);
  if(c_itr != city_map.end() ) {
    c_itr->second.visited = true;
    node_queue.push(c_itr->second);
  }
  while(!node_queue.empty()) {
  
    //Pop 1 element off queue and process it.
    Node_t node = node_queue.front();
    node_queue.pop();
    auto cs_itr=node.connectedCities.get()->begin();
    auto cs_end=node.connectedCities.get()->end();
    while(cs_itr != cs_end) {
      if(*cs_itr == city) {
	return true;
      }
      auto cm_itr = city_map.find(*cs_itr);
      if(cm_itr != city_map.end()) {
	if(cm_itr->second.visited == false) {
	  cm_itr->second.visited = true;
	  node_queue.push(cm_itr->second);
	}
      }
      ++cs_itr;
    }
  }
  return false;
};

int main(int argc, char* argv[]) {

  if(argc < 4) {
    std::cout << "Usage Error: prog filename city1 city2" << std::endl;
    return 0;
  }

  std::string   ocity1(argv[2]);
  std::string   ocity2(argv[3]);

  std::ifstream cfile(argv[1]);
  std::string   city1;
  std::string   city2;

  CitiesMap_t cityMap;
  while((std::getline(cfile, city1,'|')) && 
	(std::getline(cfile, city2,'\n'))) {
    
    insertCity(cityMap, city1, city2);
    insertCity(cityMap, city2, city1);

  };

  bool connected = false;
  if(checkCitiesExist(cityMap, ocity1, ocity2)) {
    connected = findCity(cityMap, ocity1, ocity2);
  }
  std::cout << ocity1 << " is" << ((connected) ? "" : " not") << " connected to " 
	    << ocity2 << std::endl;
  return 0;
}
