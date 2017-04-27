#include <iostream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <string>
#include <queue>
#include <fstream>
#include <istream>
#include <memory>

typedef std::unordered_set<std::string> CitiesSet_t; 
typedef CitiesSet_t::iterator           CitiesSetItr_t; 
typedef CitiesSet_t::const_iterator     CitiesSetConstItr_t; 

struct Node_t {
  bool        visited;
  std::shared_ptr<CitiesSet_t> connectedCities;
  Node_t()                  : visited(false),       connectedCities(new CitiesSet_t)     { }
  Node_t(const Node_t& rhs) : visited(rhs.visited), connectedCities(rhs.connectedCities) { }
};

typedef std::unordered_map<std::string, Node_t> CitiesMap_t;
typedef CitiesMap_t::iterator                   CitiesMapItr_t;
typedef CitiesMap_t::const_iterator             CitiesMapConstItr_t;
typedef std::queue<Node_t>                      NodeQueue_t; 

void insertCity(CitiesMap_t&       city_map, 
		const std::string& city1, 
		const std::string& city2){

  CitiesMapItr_t c_itr = city_map.find(city1);
  if(c_itr == city_map.end() ) {
    Node_t      node;
    city_map[city1]=node;
    c_itr = city_map.find(city1);
  }
  if(c_itr != city_map.end() ) {
    c_itr->second.connectedCities.get()->insert(city2);
  }
  return;
}

bool checkCitiesExist(const CitiesMap_t& city_map,
		      const std::string& city1, 
		      const std::string& city2) {
  CitiesMap_t::const_iterator c_itr1 = city_map.find(city1);
  CitiesMap_t::const_iterator c_itr2 = city_map.find(city2);
  if((c_itr1 == city_map.end()) ||
     (c_itr2 == city_map.end())) {
    return false;
  }
  if((c_itr1->second.connectedCities.get()->empty()) ||
     (c_itr2->second.connectedCities.get()->empty())) return false;
  
  return true;
}

bool findCity(CitiesMap_t&       city_map, 
	      NodeQueue_t&       node_queue,
	      const std::string& city2) {

  if(node_queue.empty()) return false;
  //Pop 1 element off queue and process it.
  Node_t node = node_queue.front();
  node_queue.pop();

  CitiesSetItr_t      cs_itr=node.connectedCities.get()->begin();
  CitiesSetConstItr_t cs_end=node.connectedCities.get()->end();
  CitiesMapItr_t      cm_itr=city_map.begin();
  CitiesMapConstItr_t cm_end=city_map.end();
  bool rv=false;
  while(cs_itr != cs_end) {
    if(*cs_itr == city2) {
      rv = true;
      break;
    }
    cm_itr = city_map.find(*cs_itr);
    if(cm_itr != cm_end) {
      if(cm_itr->second.visited == false) {
	cm_itr->second.visited = true;
	node_queue.push(cm_itr->second);
      }
    }
    ++cs_itr;
  }

  if(rv==false) {
    rv = findCity(city_map, node_queue, city2);
  }

  return rv;
};

void seedQueue(const CitiesMap_t& city_map, 
	             NodeQueue_t& node_queue, 
	       const std::string& begin_city) {

    CitiesMapConstItr_t c_itr = city_map.find(begin_city);
    if(c_itr != city_map.end() ) {
      node_queue.push(c_itr->second);
    }
    return;
}

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
    NodeQueue_t node_queue;
    seedQueue(cityMap, node_queue, ocity1);
    connected = findCity(cityMap, node_queue, ocity2);
  }

  std::cout << ocity1 << ((connected) ? " is" : " is not") << " connected to " << ocity2 << std::endl;
  return 0;
}
